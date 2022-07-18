#include <csignal>
#include <unistd.h> // sleep
#include <cstring> // error

#include "menu.hpp"

struct pidfh *Menu::pid;
Eink *Menu::eink;
bool Menu::is_selected = false;
bool Menu::next = false;

Menu::Option Menu::menu() {
  if (pid == NULL) {
    sendSignal(SIGUSR1);
    return Menu::Option::ERROR;
  }

  eink->Start();

  int animation_counter = 0;
  while (true) {
    if (is_selected)
      return options[selection];

    if (next) {
      animateNext();
      next = false;
      selection = ++selection % (sizeof(options)/sizeof(int));
      animation_counter = 0;
    } else {
      animate(animation_counter);
      animation_counter = ++animation_counter % 4;
    }
  }
}

void Menu::animate(int counter) {
  frame.Clear();

  int number_of_options = (sizeof(options)/sizeof(int));

  frame.DrawIcon(18, 37, options[(selection + number_of_options - 1) % number_of_options]);
  frame.DrawIcon(101, 37, options[selection] + counter);
  frame.DrawIcon(184, 37, options[(selection + 1) % number_of_options]);

  eink->Display(&frame);
  usleep(eink->GetDisplayDuration());
}

void Menu::animateNext() {
  frame.ShiftLeft(21);

  eink->Display(&frame);
  usleep(eink->GetDisplayDuration());

  frame.ShiftLeft(21);

  eink->Display(&frame);
  usleep(eink->GetDisplayDuration());

  frame.ShiftLeft(21);

  eink->Display(&frame);
  usleep(eink->GetDisplayDuration());
}

void Menu::sendSignal(int sig) {
  std::string old_pid;
  std::ifstream pid_file(pid_path);
  getline(pid_file, old_pid);

  kill(stoi(old_pid), sig);
}

Menu::Menu(Eink *eink) {
  pid = pidfile_open(pid_path, 0600, NULL);

  if (pid != NULL)
    pidfile_write(pid);
  else if (errno != EEXIST)
    throw std::runtime_error("Failed to lock pidfile: " + std::string(std::strerror(errno)));

  std::signal(SIGINT, sigHandler);
  std::signal(SIGUSR1, sigHandler);
  std::signal(SIGUSR2, sigHandler);

  this->eink = eink;
  selection = 0;

  cli.allow_extras(false);

  cli.add_subcommand("capture", "Capture a photo");
  cli.add_subcommand("debug", "Display debug information");
  cli.add_subcommand("menu", "Start menu or next menu item");
  cli.add_subcommand("select", "Select current menu item or start capture if menu is not active");
  cli.add_subcommand("sync", "Sync cached images into Google Photos");
}

Menu::~Menu() {
  pidfile_remove(pid);
  pid = NULL;
}

Menu::Option Menu::Parse(int argc, char *argv[]) {
  try {
    cli.parse(argc, argv);

    if (cli.got_subcommand("capture"))
      return Menu::Option::CAPTURE;

    if (cli.got_subcommand("debug"))
      return Menu::Option::DEBUG;

    if (cli.got_subcommand("menu"))
      return menu();

    if (cli.got_subcommand("select")) {
      if (pid == NULL) {
        sendSignal(SIGUSR2);
        return Menu::Option::ERROR;
      }

      return options[selection];
    }

    if (cli.got_subcommand("sync"))
      return Menu::Option::SYNC;

    throw CLI::CallForHelp();
  } catch(const CLI::ParseError &e) {
    eink->Stop();
    cli.exit(e);
  }

  return Menu::Option::ERROR;
}

void Menu::sigHandler(int sig) {
  switch (sig) {
    case SIGINT:
      eink->Stop();
      exit(EXIT_FAILURE);
      break;

    case SIGUSR1:
      next = true;
      break;

    case SIGUSR2:
      is_selected = true;
      break;

    default:
      throw std::runtime_error("Unhandled signal: " + sig);
      break;
  }
}
