#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include <stdint.h>

#include "frame.hpp"

#include "icons.hpp"
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

void Frame::DrawIcon(int x, int y, int icon) {
  Paint_SelectImage(buffer);
  Paint_DrawString_EN(x, y, std::string(1, static_cast<char>(32 + icon)).c_str(), &Icons, WHITE, BLACK);
}

void Frame::DrawText(int x, int y, std::string text) {
  Paint_SelectImage(buffer);
  Paint_DrawString_EN(x, y, text.c_str(), &Terminus16, WHITE, BLACK);
}

void Frame::SetPixel(int x, int y, bool value) {
  if (value)
    buffer[x * LINE_STRIDE + y / 8] &= ~(1UL << (7 - y % 8));
  else
    buffer[x * LINE_STRIDE + y / 8] |= 1UL << (7 - y % 8);
}

void Frame::ShiftLeft(int pixels) {
  Frame temp_frame;
  for (int i = 0; i < WIDTH; i++) {
    memcpy(temp_frame.GetBuffer() + (i * LINE_STRIDE), buffer + ((i + pixels) * LINE_STRIDE % BUFFER_SIZE), LINE_STRIDE);
  }
  memcpy(buffer, temp_frame.GetBuffer(), BUFFER_SIZE);
}
