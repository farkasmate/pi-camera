#pragma once

#include <mutex>
#include <thread>

#include "frame.hpp"

// Waveshare 2.13" EDP (250x122 rotated 270 degrees)
class Eink {
private:
  Frame current_frame;
  Frame next_frame;

  int display_duration;

  bool is_headless;
  bool is_in_shutdown;

  std::thread *display_thread;
  std::thread *init_thread;
  std::mutex mutex;

  void initialize();

  void displayHeadless();
  void displayInTheBackground();

public:
  Eink();
  ~Eink();

  int GetDisplayDuration() { return display_duration; }

  void Display(Frame *frame);

  void Start();
  void Stop();
};
