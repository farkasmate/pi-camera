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

// FIXME: remove globals
UBYTE *Image;
UWORD Imagesize = ((WIDTH % 8 == 0) ? (WIDTH / 8) : (WIDTH / 8 + 1)) * HEIGHT;

// clang-format off
static const uint8_t bayer[8][8] = {
    { 0, 32,  8, 40,  2, 34, 10, 42},
    {48, 16, 56, 24, 50, 18, 58, 26},
    {12, 44,  4, 36, 14, 46,  6, 38},
    {60, 28, 52, 20, 62, 30, 54, 22},
    { 3, 35, 11, 43,  1, 33,  9, 41},
    {51, 19, 59, 27, 49, 17, 57, 25},
    {15, 47,  7, 39, 13, 45,  5, 37},
    {63, 31, 55, 23, 61, 29, 53, 21}
};
// clang-format on

static void eink_clear() { Paint_Clear(WHITE); }

static void eink_open() {
  if ((Image = (UBYTE *)malloc(Imagesize)) == NULL)
    throw std::runtime_error("Failed to allocate eink memory");

  Paint_NewImage(Image, WIDTH, HEIGHT, 270, WHITE);
  Paint_SelectImage(Image);
  Paint_SetMirroring(MIRROR_VERTICAL);
  Paint_Clear(WHITE);
  Paint_DrawString_EN(0, 0, "Camera is", &Terminus16, WHITE, BLACK);
  Paint_DrawString_EN(0, 20, "starting...", &Terminus16, WHITE, BLACK);

  if (DEV_Module_Init() != 0)
    throw std::runtime_error("Failed to initialize eink module");
}

static void eink_draw_focus(int focus) {
  const char *focus_text = ("FOCUS: " + std::to_string(focus)).c_str();
  Paint_DrawString_EN(160, 0, focus_text, &Terminus16, WHITE, BLACK);
}

static void eink_draw_time(int time) {
  const char *time_text = ("TIME: " + std::to_string(time)).c_str();
  Paint_DrawString_EN(160, 20, time_text, &Terminus16, WHITE, BLACK);
}

static void eink_display_base() {
  EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
  EPD_2IN13_V2_DisplayPartBaseImage(Image);

  EPD_2IN13_V2_Init(EPD_2IN13_V2_PART);
}

static void eink_display_partial() { EPD_2IN13_V2_DisplayPart(Image); }

static void eink_close() {
  EPD_2IN13_V2_Sleep();
  EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
  DEV_Module_Exit();

  free(Image);
  Image = NULL;
}
