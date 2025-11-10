#[allow(unused)]
use std::cell::UnsafeCell;
use std::mem::ManuallyDrop;
use std::sync::atomic::{fence, Ordering};
use std::{ops::Deref, ptr::NonNull, sync::atomic::AtomicUsize};

struct ArcData<T> {
    data_ref_count: AtomicUsize,
    /// Number of `Arc` and `Weak`
    alloc_ref_count: AtomicUsize,
    data: UnsafeCell<ManuallyDrop<T>>,
}

pub struct MyArc<T> {
    ptr: NonNull<ArcData<T>>,
}

unsafe impl<T: Send + Sync> Send for MyArc<T> {}
unsafe impl<T: Send + Sync> Sync for MyArc<T> {}

pub struct MyWeak<T> {
    ptr: NonNull<ArcData<T>>,
}

// Only if T is Send + Sync, MyArc<T> can be Send + Sync
unsafe impl<T: Send + Sync> Send for MyWeak<T> {}
unsafe impl<T: Send + Sync> Sync for MyWeak<T> {}

impl<T> MyArc<T> {
    pub fn new(data: T) -> MyArc<T> {
        MyArc {
            ptr: NonNull::from(Box::leak(Box::new(ArcData {
                data_ref_count: AtomicUsize::new(1),
                alloc_ref_count: AtomicUsize::new(1),
                data: UnsafeCell::new(ManuallyDrop::new(data)),
            }))),
        }
    }

    fn data(&self) -> &ArcData<T> {
        unsafe { self.ptr.as_ref() }
    }

    /// Use case: MyArc::get_mut(&mut arc)
    /// Returned &mut T constraint: No other MyArc<T> can be accessible by other threads
    /// If returned &mut T is no longer exist, other MyArc<T> can be shared again
    pub fn get_mut<'a>(arc: &'a mut Self) -> Option<&'a mut T> {
        // Acquire matches Weak::drop`s Release
        if arc
            .data()
            .alloc_ref_count
            .compare_exchange(1, usize::MAX, Ordering::Acquire, Ordering::Relaxed)
            .is_err()
        {
            return None;
        }

        let is_unique = arc.data().data_ref_count.load(Ordering::Relaxed) == 1;

        // Changes after downgrade don`t change is_unique above
        arc.data().alloc_ref_count.store(1, Ordering::Release);

        if !is_unique {
            return None;
        }

        // Ensure nothing is reordered before this point
        fence(Ordering::Acquire);

        unsafe { Some(&mut *arc.data().data.get()) }
    }

    pub fn downgrade(arc: &Self) -> MyWeak<T> {
        let mut n = arc.data().alloc_ref_count.load(Ordering::Relaxed);
        loop {
            if n == usize::MAX {
                std::hint::spin_loop();
                n = arc.data().alloc_ref_count.load(Ordering::Relaxed);
                continue;
            }
            assert!(n < usize::MAX - 1);
            if let Err(e) = arc.data().alloc_ref_count.compare_exchange_weak(
                n,
                n + 1,
                Ordering::Acquire,
                Ordering::Relaxed,
            ) {
                n = e;
                continue;
            } else {
                return MyWeak { ptr: arc.ptr };
            }
        }
    }
}

impl<T> MyWeak<T> {
    fn data(&self) -> &ArcData<T> {
        unsafe { self.ptr.as_ref() }
    }

    pub fn upgrade(&self) -> Option<MyArc<T>> {
        let mut n = self.data().data_ref_count.load(Ordering::Relaxed);
        loop {
            if n == 0 {
                return None;
            }
            assert!(n < usize::MAX);
            if let Err(e) = self.data().data_ref_count.compare_exchange_weak(
                n,
                n + 1,
                Ordering::Relaxed,
                Ordering::Relaxed,
            ) {
                n = e;
            } else {
                return Some(MyArc { ptr: self.ptr });
            }
        }
    }
}

// use MyArc<T> as a reference to T
// Cannot share &mut T, so no DerefMut
impl<T> Deref for MyArc<T> {
    type Target = T;

    fn deref(&self) -> &Self::Target {
        // &self.data().data
        // SAFETY: data is only set to None when all MyArc are dropped
        unsafe { &*self.data().data.get() }
    }
}

impl<T> Clone for MyArc<T> {
    fn clone(&self) -> Self {
        // usize::MAX / 2 is an impossible number of threads for sharing Arc
        if self.data().data_ref_count.fetch_add(1, Ordering::Relaxed) > usize::MAX / 2 {
            eprintln!("Reference count overflow");
            std::process::abort();
        }
        MyArc { ptr: self.ptr }
    }
}

impl<T> Drop for MyArc<T> {
    fn drop(&mut self) {
        if self.data().data_ref_count.fetch_sub(1, Ordering::Release) == 1 {
            // Ensure all previous writes are visible before dropping the data
            std::sync::atomic::fence(Ordering::Acquire);
            // let ptr = self.weak.data().data.get();
            unsafe {
                ManuallyDrop::drop(&mut *self.data().data.get());
            }
            drop(MyWeak { ptr: self.ptr });
        }
    }
}

impl<T> Clone for MyWeak<T> {
    fn clone(&self) -> Self {
        // usize::MAX / 2 is an impossible number of threads for sharing Arc
        if self.data().alloc_ref_count.fetch_add(1, Ordering::Relaxed) > usize::MAX / 2 {
            eprintln!("Reference count overflow");
            std::process::abort();
        }
        MyWeak { ptr: self.ptr }
    }
}

impl<T> Drop for MyWeak<T> {
    fn drop(&mut self) {
        if self.data().alloc_ref_count.fetch_sub(1, Ordering::Release) == 1 {
            // Ensure all previous writes are visible before dropping the data
            std::sync::atomic::fence(Ordering::Acquire);
            unsafe {
                drop(Box::from_raw(self.ptr.as_ptr()));
            }
        }
    }
}

/// Advise: Use `Miri` to check for memory safety issues
/// `cargo +nightly miri test`
/// `cargo +nightly miri run`
#[cfg(test)]
mod tests {
    use std::sync::atomic::{AtomicUsize, Ordering};

    use super::MyArc;

    static NUM_DROPS_OPT: AtomicUsize = AtomicUsize::new(0);

    struct DetectDropMyArcOpt;

    impl Drop for DetectDropMyArcOpt {
        fn drop(&mut self) {
            NUM_DROPS_OPT.fetch_add(1, Ordering::Relaxed);
        }
    }

    #[test]
    fn test_basic() {
        NUM_DROPS_OPT.store(0, Ordering::Relaxed);
        // let a = MyArc::new(5);
        let x = MyArc::new(("test", DetectDropMyArcOpt));
        let y = x.clone();

        let t = std::thread::spawn(move || {
            assert_eq!(y.0, "test");
        });

        assert_eq!(x.0, "test");
        t.join().unwrap();

        assert_eq!(NUM_DROPS_OPT.load(Ordering::Relaxed), 0);
        drop(x);
        assert_eq!(NUM_DROPS_OPT.load(Ordering::Relaxed), 1);
    }

    #[test]
    fn test_weak() {
        NUM_DROPS_OPT.store(0, Ordering::Relaxed);
        let x = MyArc::new(("test", DetectDropMyArcOpt));
        let y = MyArc::downgrade(&x);
        let z = MyArc::downgrade(&x);

        let t = std::thread::spawn(move || {
            let upgraded = y.upgrade().unwrap();
            assert_eq!(upgraded.0, "test");
            drop(upgraded);
        });
        assert_eq!(x.0, "test");
        t.join().unwrap();

        assert_eq!(NUM_DROPS_OPT.load(Ordering::Relaxed), 0);
        assert!(z.upgrade().is_some());

        drop(x);

        assert_eq!(NUM_DROPS_OPT.load(Ordering::Relaxed), 1);
        assert!(z.upgrade().is_none());
    }
}
