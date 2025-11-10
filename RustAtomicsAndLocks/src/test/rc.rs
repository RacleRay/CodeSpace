#[cfg(test)]
mod test {
    #[test]
    fn test_rc() {
        use std::rc::Rc;

        let a = Rc::new([1, 2, 3]);
        let b = a.clone();

        assert_eq!(a.as_ptr(), b.as_ptr());
    }

    #[test]
    fn test_arc() {
        use std::sync::Arc;
        use std::thread;

        let a = Arc::new([1, 2, 3]);

        let t_handler = thread::spawn({
            let a = a.clone();
            move || {
                dbg!(a);
                a.sort();  // error
            }
        });

        dbg!(a);

        t_handler.join().unwrap();
    }
}
