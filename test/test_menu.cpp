#include <iostream>

#include "../menu/menu.hpp"

Eink eink;
Menu menu(&eink);

int main(int argc, char *argv[]) {
  switch (menu.Parse(argc, argv)) {
    case Menu::Option::CAPTURE:
      std::cout << "CAPTURE" << std::endl;
      break;

    case Menu::Option::SYNC:
      std::cout << "SYNC" << std::endl;
      break;

    case Menu::Option::DEBUG:
      std::cout << "DEBUG" << std::endl;
      break;

    default:
      exit(1);
      break;
  }

  return 0;
}
