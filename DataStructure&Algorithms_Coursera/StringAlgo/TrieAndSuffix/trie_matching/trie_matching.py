# python3
import sys
# import pysnooper

class Node:
	def __init__ (self):
		self.next = {}


def build_trie(patterns):
	root = Node()
	for pattern in patterns:
		currentNode = root
		for i in range(0, len(pattern)):
			currentSymbol = pattern[i]

			if currentSymbol in currentNode.next:
				currentNode = currentNode.next[currentSymbol]
			else:
				newNode = Node()
				currentNode.next[currentSymbol] = newNode
				currentNode = newNode

	return root

# @pysnooper.snoop()
def prefix_trie_match(text, trie_root):
	index = 0
	length = len(text)
	symbol = text[index]

	node = trie_root
	res = ''
	while True:
		if len(node.next) == 0:  # arrive leaf node
			return res

		elif symbol in node.next:  # have an edge
			res += symbol  # path record
			# next node
			node = node.next[symbol]
			# next text
			index += 1
			# 当不是leaf node时，text已经遍历结束，symbol = None
			if index < length:
				symbol = text[index]
			else:
				symbol = None

		else:
			return None


def solve (text, n, patterns):
	result = []
	root = build_trie(patterns)

	cut_index = 0
	text_ = text[cut_index: ]
	while text_:

		if prefix_trie_match(text[cut_index: ], root):
			result.append(cut_index)

		cut_index += 1
		text_ = text[cut_index: ]

	return result

# AAA
# 1
# AA

# AA
# 1
# T

# AATCGGGTTCAATCGGGGT
# 2
# ATCG
# GGGT
text = sys.stdin.readline ().strip ()
n = int (sys.stdin.readline ().strip ())
patterns = []
for i in range (n):
	patterns += [sys.stdin.readline ().strip ()]

ans = solve (text, n, patterns)

sys.stdout.write (' '.join (map (str, ans)) + '\n')
