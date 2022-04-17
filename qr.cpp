#include "dimensions.hpp"
#include "qrcodegen.hpp"

static void draw_qr(UBYTE *image, const qrcodegen::QrCode &qr) {
  for (int j = 0; j < 2 * qr.getSize(); j++) {
    for (int i = 0; i < 2 * qr.getSize(); i++) {
      if (qr.getModule(i / 2, j / 2))
        image[i * LINE_STRIDE + j / 8] &= ~(1UL << (7 - j % 8));
      else
        image[i * LINE_STRIDE + j / 8] |= 1UL << (7 - j % 8);
    }
  }
}
