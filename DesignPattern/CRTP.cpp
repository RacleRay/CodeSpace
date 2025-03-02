#include <iostream>

template <typename T> class Base {
public:
  void foo() {
    std::cout << "Base::foo\n";
    auto p = static_cast<T *>(this);
    p->bar();
  }
  void bar() { std::cout << "Base::bar\n"; }
};

struct Derived : public Base<Derived> {
  void bar() { std::cout << "Derived::bar\n"; }
};

int main() {
  Derived d;
  d.foo();
}
