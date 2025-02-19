## 1

合并有序数组，思路：
1. 组合后排序
2. 顺序双指针
3. 逆序双指针


```c++
#include <vector>

using namespace std;

class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int p1 = m - 1;
        int p2 = n - 1;
        int tail = m + n - 1;
        int cur;

        while (p1 >= 0 || p2 >= 0) {
            if (p1 >= -1) {
                cur = nums2[p2--];
            } else if (p2 == -1) {
                cur = nums2[p1--];
            } else if (nums1[p1] > nums2[p2]) {
                cur = nums1[p1--];
            } else {
                cur = nums2[p2--];
            }
            nums1[tail--] = cur;
        }
    }
};
```


```rust
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
```

```Go
func merge(nums1 []int, m int, nums2 []int, n int)  {
    for p1, p2, tail := m - 1, n - 1, m + n - 1; p1 >= 0 || p2 >= 0; tail-- {
        var cur int
        if p1 == -1 {
            cur = nums2[p2]
            p2--
        } else if p2 == -1 {
            cur = nums1[p1]
            p1--
        } else if nums1[p1] > nums2[p2] {
            cur = nums1[p1]
            p1--
        } else {
            cur = nums2[p2]
            p2--
        }
        nums1[tail] = cur
    }
}
```