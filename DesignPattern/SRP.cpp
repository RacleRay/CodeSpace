#include <iostream>
#include <vector>

struct Journal {
  std::string title;
  std::vector<std::string> entries;

  explicit Journal(const std::string &title) : title(title) {
    std::cout << "Journal created: " << title << std::endl;
  };

  void add_entry(const std::string &entry) { entries.push_back(entry); }
};

struct PersistenceManager {
  static void save(const Journal &j, const std::string &filename) {
    std::cout << "Saving Journal: " << j.title << " to " << filename
              << std::endl;
  }
};

int main() {
  Journal journal("My Journal");
  journal.add_entry("I cried today.");
  PersistenceManager::save(journal, "journal.txt");
  return 0;
}
