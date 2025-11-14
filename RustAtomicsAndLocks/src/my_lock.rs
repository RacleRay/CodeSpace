use std::{
    cell::UnsafeCell,
    ops::{Deref, DerefMut},
    sync::atomic::AtomicU32,
    sync::atomic::Ordering,
};

use atomic_wait::{wait, wake_one};

pub struct Mutex<T> {
    state: AtomicU32, // 0 unlocked, 1 locked and no other threads waiting, 2 locked and other threads waiting
    value: UnsafeCell<T>,
}

unsafe impl<T> Send for Mutex<T> {}
unsafe impl<T> Sync for Mutex<T> {}

impl<T> Mutex<T> {
    pub const fn new(value: T) -> Self {
        Self {
            state: AtomicU32::new(0),
            value: UnsafeCell::new(value),
        }
    }

    pub fn lock(&self) -> MutexGuard<T> {
        if self
            .state
            .compare_exchange(0, 1, Ordering::Acquire, Ordering::Relaxed)
            .is_err()
        {
            // locked 1 or 2
            // swap to 2 for more than one waiting
            while self.state.swap(2, Ordering::Acquire) != 0 {
                wait(&self.state, 2); // unless other waiting changed
            }
            // locked 0 -> 2
        }

        MutexGuard { mutex: self }

        // while self.state.swap(1, Ordering::Acquire) == 1 {
        //     wait(&self.state, 1); // wait unless the state is no longer 1
        // }
        // MutexGuard { mutex: self }
    }

    pub fn lock_spin(&self) -> MutexGuard<'_, T> {
        if self
            .state
            .compare_exchange(0, 1, Ordering::Acquire, Ordering::Relaxed)
            .is_err()
        {
            lock_contented(&self.state);
        }
        MutexGuard { mutex: self }
    }
}

fn lock_contented(state: &AtomicU32) {
    let mut spin_count = 0;

    // for really quic lock and unlock by other threads
    while state.load(Ordering::Relaxed) == 1 && spin_count < 100 {
        spin_count += 1;
        std::hint::spin_loop();
    }

    if state
        .compare_exchange(0, 1, Ordering::Acquire, Ordering::Relaxed)
        .is_ok()
    {
        return;
    }

    while state.swap(2, Ordering::Acquire) != 0 {
        wait(state, 2);
    }
}

pub struct MutexGuard<'a, T> {
    mutex: &'a Mutex<T>,
}

impl<T> Deref for MutexGuard<'_, T> {
    type Target = T;
    fn deref(&self) -> &T {
        unsafe { &*self.mutex.value.get() }
    }
}

impl<T> DerefMut for MutexGuard<'_, T> {
    fn deref_mut(&mut self) -> &mut T {
        unsafe { &mut *self.mutex.value.get() }
    }
}

impl<T> Drop for MutexGuard<'_, T> {
    fn drop(&mut self) {
        // only when there are some other threads waiting, we will wake one
        if self.mutex.state.swap(0, Ordering::Release) == 2 {
            wake_one(&self.mutex.state);
        }

        // self.mutex.state.store(0, Ordering::Release);
        // wake_one(&self.mutex.state);
    }
}

#[cfg(test)]
mod test {
    use std::{thread, time::Instant};

    use super::*;

    #[test]
    fn test_my_mutex() {
        let m = Mutex::new(0);
        std::hint::black_box(&m);

        let start = Instant::now();

        thread::scope(|s| {
            for _ in 0..4 {
                s.spawn(|| {
                    for _ in 0..50_000_000 {
                        *m.lock() += 1;
                    }
                });
            }
        });

        let duration = start.elapsed();
        println!("locked {} times in {:?}", *m.lock(), duration);
    }

    #[test]
    fn test_my_mutex_spin() {
        let m = Mutex::new(0);
        std::hint::black_box(&m);
        let start = Instant::now();

        thread::scope(|s| {
            for _ in 0..4 {
                s.spawn(|| {
                    for _ in 0..50_000_000 {
                        *m.lock() += 1;
                    }
                });
            }
        });

        let duration = start.elapsed();
        println!("locked {} times in {:?}", *m.lock(), duration);
    }
}
