#ifndef _418_REF_
#define _418_REF_

#include <vector>
#include <thread>
#include <mutex>

namespace ref {

  const int NUM_WORKERS = 8;
  std::vector<std::thread> workers;
  std::vector<std::function<void()>> work;
  std::mutex mtx;
  bool die = false;

  void worker() {
    while (!die) {
      if (work.size() > 0) {
        mtx.lock();
        if (work.size() > 0) {
          auto fn = work.back();
          work.pop_back();
          mtx.unlock();

          fn();
        } else {
          mtx.unlock();
        }
      }
    }
  }

  void init_runtime() {
    for (int i = 0; i < NUM_WORKERS; i++) {
      workers.push_back(std::thread(worker));
    }
  }

  void destroy_runtime() {
    die = true;
    std::for_each(workers.begin(), workers.end(), [](std::thread &worker) {
        worker.join();
      });
  }

  template<typename... Args>
  void spawn(void f(Args...), Args... args) {
    std::lock_guard<std::mutex> lock(mtx);
    work.push_back([=] () { f(args...); });
  }

  void sync() {
    while (work.size() > 0) {} // TODO: && while wokrer not idle
  }

}

#endif