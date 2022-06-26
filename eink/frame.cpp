#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include <stdint.h>

#include "frame.hpp"
#include "terminus16.hpp"

extern "C" {
#include "DEV_Config.h"
#include "GUI_Paint.h"
};

Frame::Frame() {
  Clear();

  // NOTE: width and height are switched here, because of the different coordinate systems
  Paint_NewImage(buffer, HEIGHT, WIDTH, 270, WHITE);
  Paint_SelectImage(buffer);
  Paint_SetMirroring(MIRROR_VERTICAL);
}

std::ostream& operator<<(std::ostream &stream, const Frame &frame) {
  for (int y = 0; y < frame.HEIGHT; y++) {
    for (int x = 0; x < frame.WIDTH; x++) {
      stream << ((frame.buffer[x * frame.LINE_STRIDE + y / 8] & 1UL << (7 - y % 8)) ? "#" : " ");
    }
    stream << std::endl;
  }

  return stream;
}

void Frame::Clear() {
  memset(&buffer, WHITE, BUFFER_SIZE);
}

void Frame::DrawText(int x, int y, std::string text) {
  Paint_DrawString_EN(x, y, text.c_str(), &Terminus16, WHITE, BLACK);
}

void Frame::SetPixel(int x, int y, bool value) {
  if (value)
    buffer[x * LINE_STRIDE + y / 8] &= ~(1UL << (7 - y % 8));
  else
    buffer[x * LINE_STRIDE + y / 8] |= 1UL << (7 - y % 8);
}
