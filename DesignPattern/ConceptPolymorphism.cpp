#include <iostream>

template <typename TImpl>
concept IsANotifier =
    requires(TImpl impl) { impl.AlertSMS(std::declval<std::string_view>()); };

template <IsANotifier TImpl> void Alert(TImpl &notifier, std::string_view msg) {
  notifier.AlertSMS(msg);
}

struct Notifier {
  void AlertSMS(std::string_view msg) { std::cout << msg << std::endl; }
};

int main() {
  Notifier notifier;
  Alert(notifier, "Hello");
}
