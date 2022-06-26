#pragma once

#include <chrono>

std::chrono::time_point<std::chrono::system_clock> start, stop;
std::chrono::microseconds diff;

#define START \
  start = std::chrono::high_resolution_clock::now();

#define STOP(SECTION) \
  stop = std::chrono::high_resolution_clock::now(); \
  diff = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); \
  std::cerr << "PROFILING " << diff.count() << " us (" << SECTION << ")" << std::endl;
