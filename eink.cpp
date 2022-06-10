#pragma once

#include <cstdlib>
#include <stdexcept>
#include <vector>

#include "dimensions.hpp"
#include "terminus16.hpp"

extern "C" {
#include "DEV_Config.h"
#include "GUI_Paint.h"
};

class Eink {
private:
  uint8_t *buffer;
  bool is_headless;

public:
  Eink(uint8_t *buffer, bool headless) {
    this->buffer = buffer;
    this->is_headless = headless;

    Paint_NewImage(buffer, WIDTH, HEIGHT, 270, WHITE);
    Paint_SelectImage(buffer);
    Paint_SetMirroring(MIRROR_VERTICAL);

    Clear();

    // FIXME
    Paint_DrawString_EN(0, 0, "Camera is", &Terminus16, WHITE, BLACK);
    Paint_DrawString_EN(0, 20, "starting...", &Terminus16, WHITE, BLACK);

    if (!is_headless) {
      if (DEV_Module_Init() != 0)
        throw std::runtime_error("Failed to initialize eink module");

      EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
      EPD_2IN13_V2_DisplayPartBaseImage(buffer);

      EPD_2IN13_V2_Init(EPD_2IN13_V2_PART);
    }
  }

  ~Eink() {
    if (is_headless)
      return;

    // FIXME
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_Sleep();
    DEV_Module_Exit();
  }

  void Clear() { Paint_Clear(WHITE); }

  void DrawFocus(int focus) {
    const char *focus_text = ("FOCUS: " + std::to_string(focus)).c_str();
    Paint_DrawString_EN(160, 0, focus_text, &Terminus16, WHITE, BLACK);
  }

  void DrawTime(int time) {
    const char *time_text = ("TIME: " + std::to_string(time)).c_str();
    Paint_DrawString_EN(160, 20, time_text, &Terminus16, WHITE, BLACK);
  }

  void DisplayPartial() {
    if (is_headless)
      return;

    EPD_2IN13_V2_DisplayPart(buffer);
  }
};
