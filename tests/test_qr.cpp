#include <iostream>
#include <fstream>

#include "../qr.cpp"

UBYTE *QrImage;
UBYTE *FinalImage;
UWORD Imagesize = ((WIDTH % 8 == 0) ? (WIDTH / 8) : (WIDTH / 8 + 1)) * HEIGHT;

void dump_image(UBYTE *image) {
  for (int j = 0; j < WIDTH; j++) {
    for (int i = 0; i < HEIGHT; i++) {
      if (image[i * LINE_STRIDE + j / 8] & 1UL << (7 - j % 8))
        std::cout << "#";
      else
        std::cout << "_";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void copy_image(UBYTE *from, UBYTE *to, int x, int y) {
  for (int j = 0; j + x < WIDTH; j++) {
    for (int i = 0; i + y < HEIGHT; i++) {
      if (from[i * LINE_STRIDE + j / 8] & 1UL << (7 - j % 8))
        to[(i + y) * LINE_STRIDE + (j + x) / 8] |= 1UL << (7 - (j + x) % 8);
      else
        to[(i + y) * LINE_STRIDE + (j + x) / 8] &= ~(1UL << (7 - (j + x) % 8));
    }
  }
}

int main(int argc, char *argv[]) {
  if ((QrImage = (UBYTE *)malloc(Imagesize)) == NULL)
    throw std::runtime_error("Failed to allocate QR memory");

  if ((FinalImage = (UBYTE *)malloc(Imagesize)) == NULL)
    throw std::runtime_error("Failed to allocate FINAL memory");

  const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText("https://indeed.sch.bme.hu/web/latest.jpg", qrcodegen::QrCode::Ecc::HIGH);
  draw_qr(QrImage, qr);

  copy_image(QrImage, FinalImage, 80, 110);
  dump_image(FinalImage);

  free(QrImage);
  QrImage = NULL;
  free(FinalImage);
  FinalImage = NULL;

  return 0;
}
