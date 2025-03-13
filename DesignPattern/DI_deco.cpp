#include <functional>
#include <iostream>

// 前向声明
template <typename R, typename... Args> class Decorator;

// 依赖注入
template <typename R, typename... Args> class DecoratorBuilder {
public:
  DecoratorBuilder(std::function<R(Args...)> f) : f_(f) {}

  DecoratorBuilder &before(std::function<void(Args...)> before_fn) {
    before_ = before_fn;
    return *this;
  }

  DecoratorBuilder &after(std::function<void(R)> after_fn) {
    after_ = after_fn;
    return *this;
  }

  Decorator<R, Args...> build() {
    return Decorator<R, Args...>(
        [f = f_, before = before_, after = after_](Args... args) {
          if (before)
            before(args...);
          R result = f(args...);
          if (after)
            after(result);
          return result;
        });
  }

private:
  std::function<R(Args...)> f_;
  std::function<void(Args...)> before_;
  std::function<void(R)> after_;
};

template <typename R, typename... Args> class Decorator {
public:
  Decorator(std::function<R(Args...)> f) : f_(f) {}
  R operator()(Args... args) {
    std::cout << "调用函数前" << std::endl;
    std::cout << "参数: ";
    ((std::cout << args << " "), ...);
    std::cout << std::endl;
    auto result = f_(args...);
    std::cout << "结果: " << result << std::endl;
    std::cout << "调用函数后" << std::endl;
    return result;
  }

private:
  std::function<R(Args...)> f_;
};

template <typename R, typename... Args>
Decorator<R, Args...> make_decorator(std::function<R(Args...)> f) {
  return Decorator<R, Args...>(f);
}

template <typename R, typename... Args>
DecoratorBuilder<R, Args...> make_decorator_di(std::function<R(Args...)> f) {
  return DecoratorBuilder<R, Args...>(f);
}

int main() {
  std::function<int(int)> original_function = [](int x) { return x * 2; };

  auto decorated_function = make_decorator(original_function);

  auto di_decorated_function =
      make_decorator_di(original_function)
          .before([](int x) { std::cout << "输入参数: " << x << std::endl; })
          .after([](int result) {
            std::cout << "输出结果: " << result << std::endl;
          })
          .build();

  std::cout << "测试原始装饰器：" << std::endl;
  decorated_function(5);

  std::cout << "\n测试DI装饰器: " << std::endl;
  di_decorated_function(5);
}
