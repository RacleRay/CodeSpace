from fractions import Fraction
from math import log10

# def maxnum(x):
#     maxlen = len(str(max(x)))
#     print(sorted((str(v) for v in x), reverse=True,
#                           key=lambda i: i * (maxlen // len(i))))
#     return ''.join(sorted((str(v) for v in x), reverse=True,
#                           key=lambda i: i * (maxlen * 2 // len(i))))
#
# for numbers in [(1, 34, 3, 98, 9, 76, 45, 4), (54, 546, 548, 60)]:
#     print('Numbers: %r\n  Largest integer: %15s' % (numbers, maxnum(numbers)))