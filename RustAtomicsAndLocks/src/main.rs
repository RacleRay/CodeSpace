mod test;

fn main() {
    println!("Hello, world!");

    // test::threads::test_park();
    // test::threads::test_condvar();
    // test::threads::test_atomic();
    test::threads::test_atomic_2();
}
