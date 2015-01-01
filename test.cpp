#include <iostream>
#include "CycleTimer.hpp"
#include "lib.hpp"

void spin() {
  volatile int x = 0;
  for (int i = 0; i < 10000; i++) {
    for (int j = 0; j < 10000; j++) {
      x++;
    }
  }
}

int main() {
  double start = CycleTimer::currentSeconds();

  for (int i = 0; i < 10; i++) {
    spawn(spin);
  }

  sync();
  std::cout << "Took " << CycleTimer::currentSeconds() - start << "\n";

  return 0;
}