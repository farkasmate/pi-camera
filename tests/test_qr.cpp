#include <iostream>

#define EINK_HEIGHT 250
#define EINK_WIDTH 122

uint8_t *Image;
uint16_t Imagesize = ((EINK_WIDTH % 8 == 0) ? (EINK_WIDTH / 8) : (EINK_WIDTH / 8 + 1)) * EINK_HEIGHT;

int main(int argc, char *argv[]) {
  if ((Image = (uint8_t *)malloc(Imagesize)) == NULL)
    throw std::runtime_error("Failed to allocate eink memory");

  std::cout << "hello test" << std::endl;

  return 0;
}
