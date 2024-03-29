# python3


def fibonacci_number_naive(n):
    assert 0 <= n <= 40

    if n <= 1:
        return n

    return fibonacci_number_naive(n - 1) + fibonacci_number_naive(n - 2)


def fibonacci_number(n):
    # assert 0 <= n <= 40

    if n == 0:
        return 0

    prev = 0
    curr = 1

    for i in range(1, n):
        prev, curr = curr, curr + prev

    return curr


if __name__ == '__main__':
    input_n = int(input())
    print(fibonacci_number(input_n))
