#include <iostream>
#include "lib.hpp"

void f(int x, int y){
  std::cout << x << " " << y <<"\n";
}

int main() {
  spawn(f, 3, 5);
  sync();
  return 0;
}