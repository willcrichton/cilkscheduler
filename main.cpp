#include <iostream>
#include "lib.hpp"

void f(int x, int y){
  std::cout << x << " " << y <<"\n";
}

int main() {
  lib::spawn(f, 3, 5);
  lib::sync();

  return 0;
}