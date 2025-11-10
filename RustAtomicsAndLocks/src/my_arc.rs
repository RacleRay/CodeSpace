#[allow(unused)]

use std::cell::UnsafeCell;
use std::sync::atomic::{fence, Ordering};
use std::{ops::Deref, ptr::NonNull, sync::atomic::AtomicUsize};

struct ArcData<T> {
    data_ref_count: AtomicUsize,
    /// Number of `Arc` and `Weak`
    alloc_ref_count: AtomicUsize,
    data: UnsafeCell<Option<T>>,
}

pub struct MyArc<T> {
    weak: MyWeak<T>,
}

pub struct MyWeak<T> {
    ptr: NonNull<ArcData<T>>,
}

// Only if T is Send + Sync, MyArc<T> can be Send + Sync
unsafe impl<T: Send + Sync> Send for MyWeak<T> {}
unsafe impl<T: Send + Sync> Sync for MyWeak<T> {}

impl<T> MyArc<T> {
    pub fn new(data: T) -> MyArc<T> {
        MyArc {
            weak: MyWeak {
                ptr: NonNull::from(Box::leak(Box::new(ArcData {
                    data_ref_count: AtomicUsize::new(1),
                    alloc_ref_count: AtomicUsize::new(1),
                    data: UnsafeCell::new(Some(data)),
                }))),
            },
        }
    }

    /// Use case: MyArc::get_mut(&mut arc)
    /// Returned &mut T constraint: No other MyArc<T> can be accessible by other threads
    /// If returned &mut T is no longer exist, other MyArc<T> can be shared again
    pub fn get_mut<'a>(arc: &'a mut Self) -> Option<&'a mut T> {
        if arc.weak.data().alloc_ref_count.load(Ordering::Relaxed) == 1 {
            fence(Ordering::Acquire);
            let arcdata = unsafe { arc.weak.ptr.as_mut() };
            let option = arcdata.data.get_mut();
            // SAFETY: we know there is a Arc to data
            let data = option.as_mut().unwrap();
            Some(data)
        } else {
            None
        }
    }

    pub fn downgrade(arc: &Self) -> MyWeak<T> {
        arc.weak.clone()
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
                return Some(MyArc { weak: self.clone() });
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
        let ptr = self.weak.data().data.get();
        // SAFETY: data is only set to None when all MyArc are dropped
        unsafe { (&*ptr).as_ref().unwrap() }
    }
}

impl<T> Clone for MyArc<T> {
    fn clone(&self) -> Self {
        let weak = self.weak.clone();
        // usize::MAX / 2 is an impossible number of threads for sharing Arc
        if weak.data().data_ref_count.fetch_add(1, Ordering::Relaxed) > usize::MAX / 2 {
            eprintln!("Reference count overflow");
            std::process::abort();
        }
        MyArc { weak }
    }
}

impl<T> Drop for MyArc<T> {
    fn drop(&mut self) {
        if self
            .weak
            .data()
            .data_ref_count
            .fetch_sub(1, Ordering::Release)
            == 1
        {
            // Ensure all previous writes are visible before dropping the data
            std::sync::atomic::fence(Ordering::Acquire);
            let ptr = self.weak.data().data.get();
            unsafe {
                // Set data to None to prevent further access
                (*ptr) = None;
            }
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

    static NUM_DROPS: AtomicUsize = AtomicUsize::new(0);

    struct DetectDropMyArc;

    impl Drop for DetectDropMyArc {
        fn drop(&mut self) {
            NUM_DROPS.fetch_add(1, Ordering::Relaxed);
        }
    }

    #[test]
    fn test_basic() {
        NUM_DROPS.store(0, Ordering::Relaxed);
        // let a = MyArc::new(5);
        let x = MyArc::new(("test", DetectDropMyArc));
        let y = x.clone();

        let t = std::thread::spawn(move || {
            assert_eq!(y.0, "test");
        });

        assert_eq!(x.0, "test");
        t.join().unwrap();

        assert_eq!(NUM_DROPS.load(Ordering::Relaxed), 0);
        drop(x);
        assert_eq!(NUM_DROPS.load(Ordering::Relaxed), 1);
    }

    #[test]
    fn test_weak() {
        NUM_DROPS.store(0, Ordering::Relaxed);
        let x = MyArc::new(("test", DetectDropMyArc));
        let y = MyArc::downgrade(&x);
        let z = MyArc::downgrade(&x);

        let t = std::thread::spawn(move || {
            let upgraded = y.upgrade().unwrap();
            assert_eq!(upgraded.0, "test");
        });
        assert_eq!(x.0, "test");
        t.join().unwrap();

        assert_eq!(NUM_DROPS.load(Ordering::Relaxed), 0);
        assert!(z.upgrade().is_some());

        drop(x);

        assert_eq!(NUM_DROPS.load(Ordering::Relaxed), 1);
        assert!(z.upgrade().is_none());
    }
}
