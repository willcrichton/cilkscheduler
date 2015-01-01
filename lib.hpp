#ifndef _418_LIB_
#define _418_LIB_

#include <functional>

namespace lib {

  typedef std::function<void()> Function;

  void init_runtime(){
    // no-op
  }

  template<typename... Args>
  void spawn(void f(Args...), Args... args) {
    Function fn = [&] () { f(args...); };

    // run it serially
    fn();
  }

  void sync() {
    // no-op
  }

}

#endif