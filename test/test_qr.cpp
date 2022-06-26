#include <iostream>
#include <fstream>

#include "../qr.cpp"

static void dump_eink(uint8_t *buffer, int width, int height) {
  int line_stride = height % 8 == 0 ? (height / 8) : (height / 8) + 1;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (buffer[x * line_stride + y / 8] & 1UL << (7 - y % 8))
        std::cout << "#";
      else
        std::cout << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

int main(int argc, char *argv[]) {
  Frame qrFrame;

  const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText("https://indeed.sch.bme.hu/web/latest.jpg", qrcodegen::QrCode::Ecc::HIGH);
  draw_qr(&qrFrame, qr);

  qrFrame.SetPixel(qrFrame.GetWidth() - 1, qrFrame.GetHeight() - 1);
  qrFrame.SetPixel(qrFrame.GetWidth() - 2, qrFrame.GetHeight() - 1);
  qrFrame.SetPixel(qrFrame.GetWidth() - 1, qrFrame.GetHeight() - 2);
  qrFrame.SetPixel(qrFrame.GetWidth() - 2, qrFrame.GetHeight() - 2);

  qrFrame.DrawText(100, 0, "13");
  qrFrame.DrawText(100, 20, "24");
  std::cout << qrFrame << std::endl;


  uint8_t *buffer = qrFrame.GetBuffer();
  dump_eink(buffer, qrFrame.GetWidth(), qrFrame.GetHeight());

  return 0;
}
