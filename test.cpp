#include <iostream>
#include <cmath>

#include "lib.hpp"
#include "ref.hpp"
#include "CycleTimer.hpp"

void spin() {
  volatile int x = 0;
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
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
  double refTime = time([&] () {
      ref::init_runtime();

      std::cout << "Spawning...\n";
      for (int i = 0; i < 10000; i++) {
        ref::spawn(spin);
      }

      std::cout << "Syncing...\n";
      ref::sync();
    });

  std::cout << "Reference took " << refTime << "s." << std::endl;

  double yourTime = time([&] (){
      for (int i = 0; i < 10000; i++) {
        lib::spawn(spin);
      }

      lib::sync();
    });


  std::cout << "Yours took " << yourTime << "s. ";

  if (yourTime < refTime) {
    double speedup = round(yourTime / refTime * 10000.0) / 100.0;
    std::cout << "(" << speedup << "x faster)" << std::endl;
  } else {
    double speedup = round(refTime / yourTime * 10000.0) / 100.0;
    std::cout << "(" << speedup << "x slower)" << std::endl;
  }

  return 0;
}