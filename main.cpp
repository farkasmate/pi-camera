#include <cstring>
#include <iostream>
#include <bsd/libutil.h> // pid file
#include <csignal>       // signals
#include <unistd.h>      // sleep

#include "eink/eink.hpp"
#include "menu/menu.hpp"

#include "app.cpp"

struct pidfh *pid;
Eink eink;
Menu menu(&eink);
PiCameraApp app(&eink);

void pressShutter(int sig) { app.pressShutter(); }

int main(int argc, char *argv[]) {
  switch (menu.Parse(argc, argv)) {
    case Menu::Option::CAPTURE:
      std::cout << "CAPTURE" << std::endl;

      std::signal(SIGUSR1, pressShutter);

      app.Start();

      while (true) {
        if (app.IsShutterPressed())
          break;

        std::cerr << "Sleeping 1s..." << std::endl;
        sleep(1);
      }

      app.Stop();

      break;

    case Menu::Option::SYNC:
      std::cout << "TODO: SYNC" << std::endl;
      break;

    case Menu::Option::DEBUG:
      std::cout << "TODO: DEBUG" << std::endl;
      break;

    default:
      eink.Stop();
      exit(EXIT_FAILURE);
      break;
  }

  eink.Stop();

  return EXIT_SUCCESS;
}
