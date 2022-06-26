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
  if (is_initialized)
    return;

  mutex.lock();

  if (DEV_Module_Init() != 0)
    throw std::runtime_error("Failed to initialize eink module");

  EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);

  if (is_partial) {
    EPD_2IN13_V2_DisplayPartBaseImage(frame.GetBuffer());
    EPD_2IN13_V2_Init(EPD_2IN13_V2_PART);
  }

  is_initialized = true;

  mutex.unlock();
}

void Eink::displayInTheBackground() {
  if (is_partial) {
    if (!mutex.try_lock())
      return;

    EPD_2IN13_V2_DisplayPart(frame.GetBuffer());
  }
  else {
    mutex.lock();

    EPD_2IN13_V2_Display(frame.GetBuffer());
  }

  mutex.unlock();
}

Eink::Eink(bool headless, bool partial) {
  is_headless = headless;
  is_partial = partial;

  if (!is_headless)
    initialize();
}

Eink::~Eink() {
  if (is_headless)
    return;

  SetPartial(false);
  DEV_Module_Exit();
}

void Eink::Display() {
  if (is_headless)
    return;

  if (!is_initialized)
    return;

  std::thread *display_thread = new std::thread(&Eink::displayInTheBackground, this);
  display_thread->detach();
}

void Eink::SetPartial(bool partial) {
  if (is_partial == partial)
    return;

  mutex.lock();

  is_partial = partial;

  if (is_partial) {
    EPD_2IN13_V2_DisplayPartBaseImage(frame.GetBuffer());
    EPD_2IN13_V2_Init(EPD_2IN13_V2_PART);
  }
  else
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);

  mutex.unlock();
}
