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

    EPD_2IN13_V2_DisplayPart(current_frame.GetBuffer());
  }
}

Eink::Eink() {
  display_thread = NULL;
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

  display_thread = new std::thread(&Eink::displayInTheBackground, this);
}

void Eink::Stop() {
  is_in_shutdown = true;
  display_thread->join();

  mutex.lock();
  display_thread = NULL;
  DEV_Module_Exit();
  mutex.unlock();
}
