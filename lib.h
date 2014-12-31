#include <functional>

template<typename... Args>
void spawn(void f(Args...), Args... args) {
  std::function<void()> fn = [&] () { f(args...); };
  fn();
}

void sync() {
  // no-op
}