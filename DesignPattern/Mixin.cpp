#include <iostream>

template <typename T> struct Base {
  void bar() { std::cout << "Base::bar\n"; }
};

template <typename T> struct Mixin : T {
  void foo() { std::cout << "Mixin::foo\n"; }
};

int main() {
  Mixin<Base<int>> m;
  m.foo();
  m.bar();
}
