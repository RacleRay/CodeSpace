"""
给定一个只包括 '('，')'，'{'，'}'，'['，']' 的字符串，判断字符串是否有效。

有效字符串需满足：

左括号必须用相同类型的右括号闭合。
左括号必须以正确的顺序闭合。
注意空字符串可被认为是有效字符串。

示例 1:

输入: "()"
输出: true
示例 2:

输入: "()[]{}"
输出: true
示例 3:

输入: "(]"
输出: false
示例 4:

输入: "([)]"
输出: false
示例 5:

输入: "{[]}"
输出: true

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/valid-parentheses
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
"""
class Solution(object):
    def isValid(self, s):
        """
        :type s: str
        :rtype: bool
        """
        stack = []
        pair_map = {')':'(','}':'{',']':'['}  # 注意顺序，需要先压入左括号，右括号匹配
        for c in s:
            if c not in pair_map:
                stack.append(c)
            # 为空：c找不到匹配，不为空：匹配不满足
            elif not stack or pair_map[c] != stack.pop():
                return False
        # 最后是否为空, 为空：not stack==True
        return not stack