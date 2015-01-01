#ifndef _418_REF_
#define _418_REF_

namespace ref {

  void init_runtime() {
    // no-op
  }

  void destroy_runtime() {
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