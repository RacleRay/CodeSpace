impl Solution {
    pub fn merge(nums1: &mut Vec<i32>, m: i32, nums2: &mut Vec<i32>, n: i32) {
        let mut p1 = m as usize;
        let mut p2 = n as usize;
        let mut tail = (m + n) as usize;

        while p1 > 0 || p2 > 0 {
            tail -= 1;
            if p1 == 0 {
                nums1[tail] = nums2[p2 - 1];
                p2 -= 1;
            } else if p2 == 0 {
                nums1[tail] = nums1[p1 - 1];
                p1 -= 1;
            } else if nums1[p1 - 1] > nums2[p2 - 1] {
                nums1[tail] = nums1[p1 - 1];
                p1 -= 1;
            } else {
                nums1[tail] = nums2[p2 - 1];
                p2 -= 1;
            }
        }
    }
}