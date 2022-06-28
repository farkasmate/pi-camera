#include <iostream>
#include <bsd/libutil.h> // pid file
#include <csignal>       // signals
#include <unistd.h>      // sleep

#include "app.cpp"

struct pidfh *pid;
PiCameraApp app;

void sig_handler(int sig) {
  switch (sig) {
    case SIGINT:
      pidfile_remove(pid);
      exit(EXIT_FAILURE);
      break;

    case SIGUSR1:
      app.pressShutter();
      break;

    case SIGUSR2:
      // TODO
      break;

    default:
      throw std::runtime_error("Unhandled signal: " + sig);
      break;
  }
}

int main(int argc, char *argv[]) {
  std::signal(SIGINT, sig_handler);
  std::signal(SIGUSR1, sig_handler);
  //std::signal(SIGUSR2, sig_handler);

  pid = pidfile_open("/var/run/pi_camera.pid", 0600, NULL);

  if (errno == EACCES)
    throw std::runtime_error("Failed to lock pidfile");

  pidfile_write(pid);

  app.Run();

  while (true) {
    if (app.IsShutterPressed())
      break;

    std::cerr << "Sleeping 1s..." << std::endl;
    sleep(1);
  }

  pidfile_remove(pid);
  pid = NULL;

  return EXIT_SUCCESS;
}
