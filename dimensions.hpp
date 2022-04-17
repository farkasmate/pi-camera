#pragma once

extern "C" {
#include "EPD_2in13_V2.h"
};

#ifndef HEIGHT
#define HEIGHT EPD_2IN13_V2_HEIGHT
#endif

#ifndef WIDTH
#define WIDTH EPD_2IN13_V2_WIDTH
#endif

#define LINE_STRIDE (WIDTH % 8 == 0 ? WIDTH / 8 : (WIDTH + 8) / 8)
