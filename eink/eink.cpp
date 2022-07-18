#include <chrono>
#include <cstdlib>
#include <stdexcept>
#include <vector>

#include "eink.hpp"
#include "frame.hpp"

extern "C" {
#include "DEV_Config.h"
#include "EPD_2in13_V2.h"
};

void Eink::initialize() {
  mutex.lock();

  if (DEV_Module_Init() != 0)
    throw std::runtime_error("Failed to initialize eink module");

  // NOTE: Initialize as partial
  EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
  EPD_2IN13_V2_DisplayPartBaseImage(current_frame.GetBuffer());
  EPD_2IN13_V2_Init(EPD_2IN13_V2_PART);

  mutex.unlock();
}

void Eink::displayHeadless() {
  while (true) {
    mutex.lock();
    memcpy(current_frame.GetBuffer(), next_frame.GetBuffer(), current_frame.Size());
    mutex.unlock();

    if (is_in_shutdown)
      return;

    std::cout << current_frame;
    usleep(display_duration);
  }
}

void Eink::displayInTheBackground() {
  if (init_thread != NULL) {
    init_thread->join();
    init_thread = NULL;
  }

  while (true) {
    mutex.lock();
    memcpy(current_frame.GetBuffer(), next_frame.GetBuffer(), current_frame.Size());
    mutex.unlock();

    if (is_in_shutdown) {
      EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
      EPD_2IN13_V2_Display(current_frame.GetBuffer());

      return;
    }

    std::chrono::time_point<std::chrono::system_clock> start, stop;
    start = std::chrono::high_resolution_clock::now();

    EPD_2IN13_V2_DisplayPart(current_frame.GetBuffer());

    stop = std::chrono::high_resolution_clock::now();
    std::chrono::microseconds diff = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    display_duration = diff.count();

    // TODO: Optimize?
    //std::cerr << "DISPLAY TOOK: " << display_duration << "us" << std::endl; std::cerr.flush();
  }
}

Eink::Eink() {
  display_duration = 200000;
  display_thread = NULL;

  if (getenv("PI_CAMERA_HEADLESS") && strcmp(getenv("PI_CAMERA_HEADLESS"), "true") == 0) {
    is_headless = true;
    init_thread = NULL;
    return;
  }

  is_headless = false;
  init_thread = new std::thread(&Eink::initialize, this);
}

Eink::~Eink() {
  if (display_thread != NULL)
    Stop();
}

void Eink::Display(Frame *frame) {
  mutex.lock();

  memcpy(next_frame.GetBuffer(), frame->GetBuffer(), next_frame.Size());

  mutex.unlock();
}

void Eink::Start() {
  if (display_thread != NULL)
    return;

  mutex.lock();

  // NOTE: Check again after obtaining mutex
  if (display_thread != NULL)
    return;

  if (is_headless)
    display_thread = new std::thread(&Eink::displayHeadless, this);
  else
    display_thread = new std::thread(&Eink::displayInTheBackground, this);

  mutex.unlock();
}

void Eink::Stop() {
  if (is_in_shutdown || display_thread == NULL)
    return;

  is_in_shutdown = true;
  display_thread->join();

  mutex.lock();
  display_thread = NULL;
  if (!is_headless)
    DEV_Module_Exit();
  mutex.unlock();
}
