#pragma once

#include <iostream>

class Frame {
private:
  static const int WIDTH = 250;
  static const int HEIGHT = 122;

  static const int LINE_STRIDE = HEIGHT % 8 == 0 ? (HEIGHT / 8) : (HEIGHT / 8) + 1;
  static const int BUFFER_SIZE = LINE_STRIDE * WIDTH;

  uint8_t buffer[BUFFER_SIZE];

public:
  Frame();

  friend std::ostream& operator<<(std::ostream &stream, const Frame &frame);

  int GetWidth() { return WIDTH; }
  int GetHeight() { return HEIGHT; }
  uint8_t* GetBuffer() { return buffer; }
  int Size() { return BUFFER_SIZE; }

  void Clear();
  void DrawIcon(int x, int y, int icon);
  void DrawText(int x, int y, std::string text);
  // NOTE: true == BLACK
  void SetPixel(int x, int y, bool value = true);
  void ShiftLeft(int pixels);
};
