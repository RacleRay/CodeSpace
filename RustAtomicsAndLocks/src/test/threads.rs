use std::collections::VecDeque;
use std::sync::Mutex;
use std::thread;
use std::time::Duration;

fn test_leak() {
    let x: &'static [i32; 3] = Box::leak(Box::new([1, 2, 3]));

    // reference 实现了 Copy，如果你move它们，原始的仍然存在。就像integer或boolean那样。
    thread::spawn(move || dbg!(x));
    thread::spawn(move || dbg!(x));

    // thread::spawn(|| {
    //     let y = x;
    //     dbg!(y);
    // });
}

pub(crate) fn test_park() {
    let queue = Mutex::new(VecDeque::new());

    // there is some bug here
    thread::scope(|s| {
        // Consuming thread
        let t = s.spawn(|| loop {
            let item = queue.lock().unwrap().pop_front();
            if let Some(item) = item {
                dbg!(item);
            } else {
                thread::park();
                dbg!("parked");
            }
        });

        // Producing thread
        for i in 0.. {
            queue.lock().unwrap().push_back(i);
            t.thread().unpark();
            dbg!("unparked");
            thread::sleep(Duration::from_secs(1));
        }
    });
}

pub(crate) fn test_condvar() {
    use std::sync::Condvar;

    let queue = Mutex::new(VecDeque::new());
    let not_empty = Condvar::new();

    thread::scope(|s| {
        s.spawn(|| loop {
            let mut q = queue.lock().unwrap();
            let item = loop {
                if let Some(item) = q.pop_front() {
                    break item;
                } else {
                    q = not_empty.wait(q).unwrap();
                }
            };
            drop(q);
            dbg!(item);
        });

        for i in 0.. {
            let mut q = queue.lock().unwrap();
            q.push_back(i);
            not_empty.notify_one();
        }
    })
}

pub(crate) fn test_atomic() {
    use std::sync::atomic::{AtomicUsize, Ordering::Relaxed};

    let num_done = &AtomicUsize::new(0);

    let process_item = |i| {
        let mut res = 0u64;
        for i in 0..10_000_000 {
            res += i;
        }
        // dbg!(i, res);
        res
    };

    thread::scope(|s| {
        // Four background threads to process all 100 items, 25 each.
        for t in 0..4 {
            s.spawn(move || {
                for i in 0..25 {
                    process_item(t * 25 + i); // Assuming this takes some time.
                    num_done.fetch_add(1, Relaxed);
                }
            });
        }

        // The main thread shows status updates, every second.
        loop {
            let n = num_done.load(Relaxed);
            if n == 100 {
                break;
            }
            println!("Working.. {n}/100 done");
            thread::sleep(Duration::from_secs(1));
        }
    });

    println!("Done!");
}

// This statics is not precise ! not right
pub(crate) fn test_atomic_2() {
    use std::sync::atomic::{AtomicU64, AtomicUsize, Ordering::Relaxed};
    use std::time::Instant;

    let process_item = |i| {
        let mut res = 0u64;
        for i in 0..10_000_000 {
            res += i;
        }
        // dbg!(i, res);
        res
    };

    let num_done = &AtomicUsize::new(0);
    let total_time = &AtomicU64::new(0);
    let max_time = &AtomicU64::new(0);

    thread::scope(|s| {
        // Four background threads to process all 100 items, 25 each.
        for t in 0..4 {
            s.spawn(move || {
                for i in 0..25 {
                    let start = Instant::now();
                    process_item(t * 25 + i); // Assuming this takes some time.
                    let time_taken = start.elapsed().as_micros() as u64;
                    num_done.fetch_add(1, Relaxed);
                    total_time.fetch_add(time_taken, Relaxed);
                    max_time.fetch_max(time_taken, Relaxed);
                }
            });
        }

        // The main thread shows status updates, every second.
        loop {
            let total_time = Duration::from_micros(total_time.load(Relaxed));
            let max_time = Duration::from_micros(max_time.load(Relaxed));
            let n = num_done.load(Relaxed);
            if n == 100 {
                println!(
                    "Working.. {n}/100 done, {:?} average, {:?} peak",
                    total_time, max_time
                );
                break;
            }
            if n == 0 {
                println!("Working.. nothing done yet.");
            } else {
                println!(
                    "Working.. {n}/100 done, {:?} average, {:?} peak",
                    total_time / n as u32,
                    max_time,
                );
            }
            thread::sleep(Duration::from_secs(1));
        }
    });

    println!("Done!");
}

#[cfg(test)]
mod test {
    #[test]
    fn test_leak() {
        super::test_leak();
    }

    #[test]
    fn test_park() {
        super::test_park();
    }
}
