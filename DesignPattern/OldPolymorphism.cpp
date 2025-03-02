#include <iostream>

template <typename TImpl> class Notifier {
public:
  void AlertSMS(std::string_view msg) { impl().AlertSMS(msg); }

private:
  TImpl &impl() { return static_cast<TImpl &>(*this); }
  friend TImpl;
};

template <typename TImpl>
void Alert(Notifier<TImpl> &notifier, std::string_view msg) {
  notifier.AlertSMS(msg);
}

struct TestNotifier : Notifier<TestNotifier> {
  void AlertSMS(std::string_view msg) { std::cout << msg << std::endl; }
};

int main() {
  TestNotifier notifier;
  Alert(notifier, "Hello");
  return 0;
}
