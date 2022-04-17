#include "dimensions.hpp"
#include "qrcodegen.hpp"

static void draw_qr(UBYTE *image, const qrcodegen::QrCode &qr) {
  int border = 4;
  for (int j = -border; j < 2 * qr.getSize() + border; j++) {
    for (int i = -border; i < 2 * qr.getSize() + border; i++) {
      if (qr.getModule(i / 2, j / 2))
        image[i * LINE_STRIDE + j / 8] &= ~(1UL << (7 - j % 8));
      else
        image[i * LINE_STRIDE + j / 8] |= 1UL << (7 - j % 8);
    }
  }
}