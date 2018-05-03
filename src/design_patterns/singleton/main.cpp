// 单例模式
#include <iostream>
#include <string>

class Singleton final {
public:
  static Singleton &GetInstance() {
    static Singleton instance; // !c++11保证局部静态变量线程安全
    return instance;
  }

  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;

private:
  Singleton() = default;
};

int main(int argc, char const *argv[]) {
  const Singleton &singleton1 = Singleton::GetInstance();
  const Singleton &singleton2 = Singleton::GetInstance();

  assert(&singleton1 == &singleton2); // same address, point to same object.
}
