mod stack_list;
pub mod deque;
pub mod double;
pub mod first;
pub mod second;
pub mod third;
pub mod unsafell;

pub fn add(left: u64, right: u64) -> u64 {
    left + right
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(result, 4);

        let mut data = 10;
        let ref1 = &mut data;
        let ref2 = &mut *ref1;

        // 借用者的借用有效范围并不是看作用域，而是看最后一次使用的位置
        *ref2 += 2;
        *ref1 += 1;

        // println!("ref1: {}, ref2: {}", ref1, ref2);

        unsafe {
            let mut data = 10;
            let ref1 = &mut data;
            let ptr2 = ref1 as *mut _;
            let ref3 = &mut *ptr2;
            let ptr4 = ref3 as *mut _;

            // *ptr2 += 2;    // UB if not comment this

            *ptr4 += 4;
            *ref3 += 3;
            *ptr2 += 2;
            *ref1 += 1;

            println!("{}", data);
        }

        unsafe {
            let mut data = 10;
            let ref1 = &mut data;
            let ptr2 = ref1 as *mut _;
            let ref3 = &mut *ptr2;
            let ptr4 = ref3 as *mut _;

            *ptr4 += 4;
            *ref3 += 3;
            *ptr2 += 2;
            *ref1 += 1;

            println!("{}", data);
        }

        use std::cell::UnsafeCell;

        fn opaque_read(val: &i32) {
            println!("val: {}", *val);
        }

        unsafe {
            let mut data = UnsafeCell::new(10);
            let  mref1 = &mut data;
            let ptr2 = mref1.get();
            let sref3 = &*mref1;

            *ptr2 += 2;
            opaque_read(&*sref3.get());
            *sref3.get() += 3;
            *mref1.get() += 1;

            println!("{}", *data.get());
        }
    }
}
