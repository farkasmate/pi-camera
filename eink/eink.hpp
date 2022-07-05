#pragma once

#include <mutex>
#include <thread>

#include "frame.hpp"

// Waveshare 2.13" EDP (250x122 rotated 270 degrees)
class Eink {
private:
  Frame current_frame;
  Frame next_frame;

  bool is_in_shutdown;

  std::thread *display_thread;
  std::thread *init_thread;
  std::mutex mutex;

  void initialize();
  void displayInTheBackground();

public:
  Eink();
  ~Eink();

  void Display(Frame *frame);

  void Start();
  void Stop();
};
