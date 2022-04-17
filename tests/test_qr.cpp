#include <iostream>
#include <fstream>

#define UBYTE uint8_t
#define UWORD uint16_t

#include "../qr.cpp"

UBYTE *Image;
UWORD Imagesize = ((WIDTH % 8 == 0) ? (WIDTH / 8) : (WIDTH / 8 + 1)) * HEIGHT;

void dump_image(UBYTE *image) {
  for (int j = 0; j < WIDTH; j++) {
    for (int i = 0; i < HEIGHT; i++) {
      bool black = image[i * LINE_STRIDE + j / 8] & 1UL << (7 - j % 8);
      if (black)
        std::cout << "#";
      else
        std::cout << "_";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

int main(int argc, char *argv[]) {
  if ((Image = (UBYTE *)malloc(Imagesize)) == NULL)
    throw std::runtime_error("Failed to allocate eink memory");

  const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText("https://indeed.sch.bme.hu/web/latest.jpg", qrcodegen::QrCode::Ecc::HIGH);
  draw_qr(Image, qr);

  dump_image(Image);

  return 0;
}
