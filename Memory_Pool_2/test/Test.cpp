#include <cassert>
#include <iostream>

constexpr int A = 8;

int main() {
    for (int i = 1; i < 100; i++) {
        int a = (((i + A - 1) & ~(A - 1)) / A - 1);
        int b = (i + A - 1) / A - 1;
        int c = i / A;
        assert(a == b);
        if (a != b) {
            std::cout << "ab " << i << " " << a << " " << b << std::endl;
        }
        if (b != c) {
            std::cout << "bc " << i << " " << b << " " << c << std::endl;
        }
    }
}