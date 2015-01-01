#ifndef _418_LIB_
#define _418_LIB_

namespace lib {

  void init_runtime(){
    // no-op
  }

  template<typename... Args>
  void spawn(void f(Args...), Args... args) {
    // run it serially
    f(args...);
  }

  void sync() {
    // no-op
  }

}

#endif