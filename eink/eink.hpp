#pragma once

#include <mutex>
#include <thread>

#include "frame.hpp"

// Waveshare 2.13" EDP (250x122 rotated 270 degrees)
class Eink {
private:
  Frame frame;

  bool is_headless;
  bool is_initialized;
  bool is_partial;

  std::mutex mutex;

  void initialize();
  void displayInTheBackground();

public:
  Eink(bool headless, bool partial);
  ~Eink();

  Frame* GetFrame() { return &frame; }

  std::thread* Display();

  void SetPartial(bool partial);
};
