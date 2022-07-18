#include <iostream>

#include <unistd.h>

#include "../eink/frame.hpp"

int main(int argc, char *argv[]) {
  Frame frame;

  frame.DrawIcon(0, 0, 0);
  std::cout << frame << std::endl;

  return 0;
}
