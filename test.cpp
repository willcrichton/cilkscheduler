#include <iostream>
#include <thread>
#include <cmath>

#include "lib.hpp"
#include "ref.hpp"
#include "util/CycleTimer.hpp"

// TODO: add more tests + correctness check

void spin() {
  volatile int x = 0;
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      x++;
    }
  }
}

double time(std::function<void()> to_test) {
  double start = CycleTimer::currentSeconds();
  to_test();
  return CycleTimer::currentSeconds() - start;
}


int main() {
  std::cout << "Tests started on " << std::thread::hardware_concurrency() << " cores." << std::endl;

  double refTime = time([&] () {
      ref::init_runtime();

      for (int i = 0; i < 10000; i++) {
        ref::spawn(spin);
      }

      ref::sync();
      ref::destroy_runtime();
    });

  std::cout << "Reference took " << refTime << "s." << std::endl;

  double yourTime = time([&] (){
      lib::init_runtime();

      std::cout << "Spawning\n";
      for (int i = 0; i < 10000; i++) {
        lib::spawn(spin);
      }

      std::cout << "Syncing\n";
      lib::sync();
      std::cout << "Destroying\n";
      lib::destroy_runtime();
    });


  std::cout << "Yours took " << yourTime << "s. ";

  if (yourTime < refTime) {
    double speedup = round(refTime / yourTime * 10000.0) / 10000.0;
    std::cout << "(" << speedup << "x faster)" << std::endl;
  } else {
    double speedup = round(yourTime / refTime * 10000.0) / 10000.0;
    std::cout << "(" << speedup << "x slower)" << std::endl;
  }

  return 0;
}