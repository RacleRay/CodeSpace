# python3
import sys


def computePrefixFunction(pattern):
  s = [0 for _ in range(len(pattern))]
  border = 0
  for i in range(1, len(pattern)):
    # 匹配border冲突，以前缀border-1位置的值再匹配。后匹配前
    while border > 0 and pattern[i] != pattern[border]:
      border = s[border - 1]

    if pattern[i] == pattern[border]:
      border += 1

    else:
      border = 0

    s[i] = border

  return s


def find_pattern(pattern, text):
  """
  Find all the occurrences of the pattern in the text
  and return a list of all positions in the text
  where the pattern starts in the text.
  """
  result = []
  concat = pattern + '$' + text
  prefixFunc = computePrefixFunction(concat)

  for i in range(len(pattern) + 1, len(concat)):
    # 匹配到pattern相同的occurence
    if prefixFunc[i] == len(pattern):
      result.append(i - 2 * len(pattern))

  return result


if __name__ == '__main__':
  # ATA
  # ATATA

  # ATAT
  # GATATATGCATATACTT
  pattern = sys.stdin.readline().strip()
  text = sys.stdin.readline().strip()
  result = find_pattern(pattern, text)
  print(" ".join(map(str, result)))

