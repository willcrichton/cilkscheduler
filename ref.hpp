#ifndef _418_REF_
#define _418_REF_

#include <functional>
#include <vector>
#include <thread>

namespace ref {

  typedef std::function<void()> Function;

  std::vector<std::thread> threads;

  void init_runtime() {
    // no-op
  }

  template<typename... Args>
  void spawn(void f(Args...), Args... args) {
    Function fn = [=] () { f(args...); };

    // run it serially
    threads.push_back(std::thread(fn));
  }

  void sync() {
    std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
        t.join();
      });
  }

}

#endif