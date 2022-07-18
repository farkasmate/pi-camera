#pragma once

#include <bsd/libutil.h> // pid

#include "../eink/eink.hpp"

#include "CLI11.hpp"

class Menu {
public:
  enum Option {
    CAPTURE = 0,
    DEBUG = 4,
    SYNC = 8,

    ERROR = -1,
  };

private:
  static struct pidfh *pid;
  const char *pid_path = "/var/run/pi_camera.pid";

  static Eink *eink;
  Frame frame;

  CLI::App cli;

  int selection;
  static bool is_selected;
  static bool next;
  const Option options[3] = { CAPTURE, SYNC, DEBUG };

  void animate(int counter);
  void animateNext();
  Option menu();

  void sendSignal(int sig);

public:
  Menu(Eink *eink);
  ~Menu();

  Option Parse(int argc, char *argv[]);

  static void sigHandler(int sig);
};
