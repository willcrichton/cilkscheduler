#ifndef _418_REF_
#define _418_REF_

#include <vector>
#include <thread>
#include <mutex>

namespace ref {

  const int NUM_WORKERS = 8;
  std::vector<std::thread> workers(NUM_WORKERS);
  std::vector<std::vector<std::function<void()>>> workQueues(NUM_WORKERS);
  std::vector<std::mutex> locks(NUM_WORKERS);
  bool die = false;

  void worker(int workerId) {
    std::vector<std::function<void()>> &queue = workQueues[workerId];
    std::mutex &lock = locks[workerId];

    while (!die) {
      if (queue.size() > 0) {
        lock.lock();
        if (queue.size() > 0) {
          auto fn = queue.back();
          queue.pop_back();
          lock.unlock();

          fn();
        } else {
          lock.unlock();
        }
      }
    }
  }

  void init_runtime() {
    for (int i = 0; i < NUM_WORKERS; i++) {
      workers[i] = std::thread([=](){ worker(i); });
    }
  }

  void destroy_runtime() {
    die = true;
    std::for_each(workers.begin(), workers.end(), [](std::thread &worker) {
        worker.join();
      });
  }

  // TODO: make spawn thread safe
  int curThread = 0;

  template<typename... Args>
  void spawn(void f(Args...), Args... args) {
    std::lock_guard<std::mutex> lock(locks[curThread]);
    workQueues[curThread].push_back([=] () { f(args...); });
    curThread = (curThread + 1) % NUM_WORKERS;
  }

  void sync() {
    while (true) {
      bool allDone = true;
      for (int i = 0; i < workQueues.size(); i++) {
        if (workQueues[i].size() > 0) { // TODO: && worker is idle
          allDone = false;
          break;
        }
      }

      if (allDone) return;
    }
  }

}

#endif