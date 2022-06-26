#include "eink/frame.hpp"
#include "qrcodegen.hpp"

static void draw_qr(Frame *frame, const qrcodegen::QrCode &qr) {
  for (int j = 0; j < 2 * qr.getSize(); j++) {
    for (int i = 0; i < 2 * qr.getSize(); i++) {
      if (qr.getModule(i / 2, j / 2))
        frame->SetPixel(i, j);
    }
  }
}
