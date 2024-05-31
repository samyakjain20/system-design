#include<bits/stdc++.h>
using namespace std;

class Singleton {
private:
  Singleton(const std::string value) : value_(value) {}
  static Singleton* instance_;
  std::string value_;

public:
  static Singleton* GetInstance(const std::string& value) {
    if (instance_ == nullptr) {
      instance_ = new Singleton(value);
    }
    return instance_;
  }

  std::string value() const { return value_; }

  // Disallow copying and assignment
  Singleton(Singleton&) = delete;
  void operator=(const Singleton&) = delete;
};
Singleton* Singleton::instance_ = nullptr;



int main() {
  std::cout << "Singleton Example:\n";

  std::thread t1([] {
    Singleton* singleton = Singleton::GetInstance("FOO");
    std::cout << singleton->value() << "\n";
  });

  std::thread t2([] {
    Singleton* singleton = Singleton::GetInstance("BAR");
    std::cout << singleton->value() << "\n";
  });

  t1.join();
  t2.join();

  return 0;
}
