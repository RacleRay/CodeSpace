"""
给定一个整数数组 nums 和一个目标值 target，请你在该数组中找出和为目标值的那 两个 整数，并返回他们的数组下标。

你可以假设每种输入只会对应一个答案。但是，你不能重复利用这个数组中同样的元素。

示例:

给定 nums = [2, 7, 11, 15], target = 9

因为 nums[0] + nums[1] = 2 + 7 = 9
所以返回 [0, 1]

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/two-sum
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
"""
class Solution:
    def twoSum(self, nums, target: int):

        # _dict = {}
        # for i, m in enumerate(nums):
        #     if _dict.get(target - m) is not None:
        #         return [i, _dict.get(target - m)]
        #     _dict[m] = i

        # _dict = {}
        # for i in range(len(nums)):
        #     if target-nums[i] in _dict:
        #         return [i, _dict[target - nums[i]]]
        #     _dict[nums[i]] = i

        if len(nums) <= 1:
            return False
        buff_dict = {}
        for i in range(len(nums)):
            if nums[i] not in buff_dict:
                buff_dict[target - nums[i]] = i
            else:
                return [buff_dict[nums[i]], i]
        return None


a = Solution()
b = a.twoSum([2,7,11,15], 9)
print(b)