#ifndef _418_REF_
#define _418_REF_

#include <vector>
#include <thread>

namespace ref {

  std::vector<std::thread> threads;

  void init_runtime() {
    // no-op
  }

  void destroy_runtime() {
    // no-op
  }

  template<typename... Args>
  void spawn(void f(Args...), Args... args) {
    threads.push_back(std::thread([=] () { f(args...); }));
  }

  void sync() {
    std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
        t.join();
      });
  }

}

#endif