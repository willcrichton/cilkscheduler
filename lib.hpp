#ifndef _418_LIB_
#define _418_LIB_

#include <functional>

typedef std::function<void()> Function;

template<typename... Args>
void spawn(void f(Args...), Args... args) {
  Function fn = [&] () { f(args...); };

  // run it serially
  fn();
}

void sync() {
  // no-op
}

#endif