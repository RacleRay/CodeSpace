#include <iostream>

class Persion {
  int age_;

public:
  int get_age() { return age_; }
  void set_age(int a) { age_ = a; }
  __declspec(property(get = get_age, put = set_age)) int age;
};

int main() {
  Persion p;
  p.age = 10;
  std::cout << p.age << std::endl;
  return 0;
}
