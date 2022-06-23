#include <csignal>
#include <fstream>
#include <stdexcept>

#include <bsd/libutil.h>
#include <signal.h>

class SigHandler {
private:
  std::string path;
  void (*sigusr1_handler)(int);
  void (*sigusr2_handler)(int);

public:
  static struct pidfh *pid;

  static void sigint_handler(int signo) {
    pidfile_remove(pid);

    exit(1);
  }

  static void hook(std::string path, void (*sigusr1_handler)(int), void (*sigusr2_handler)(int)) {
    static SigHandler instance = SigHandler(path, sigusr1_handler, sigusr2_handler);

    if (instance.path != path)
      throw std::runtime_error("SigHandler is already configured with a different PID file: " + instance.path);

    if (instance.sigusr1_handler != sigusr1_handler)
      throw std::runtime_error("SigHandler is already configured with a different USR1 handler");

    if (instance.sigusr2_handler != sigusr2_handler)
      throw std::runtime_error("SigHandler is already configured with a different USR2 handler");
  }

  SigHandler(SigHandler const &) = delete;
  void operator=(SigHandler const &) = delete;

private:
  SigHandler(std::string path, void (*sigusr1_handler)(int), void (*sigusr2_handler)(int)) {
    this->path = path;
    this->sigusr1_handler = sigusr1_handler;
    this->sigusr2_handler = sigusr2_handler;

    std::signal(SIGINT, sigint_handler);
    std::signal(SIGUSR1, sigusr1_handler);
    std::signal(SIGUSR2, sigusr2_handler);

    pid = pidfile_open(path.c_str(), 0600, NULL);

    if (pid == NULL) {
      if (errno == EACCES)
        throw std::runtime_error("Permission denied: " + path);
      else if (errno == EEXIST) {
        std::string old_pid;
        std::ifstream pid_file(path);
        getline(pid_file, old_pid);

        kill(stoi(old_pid), SIGUSR1);
        throw std::runtime_error("Process is already running, USR1 sent");
      }
    }

    pidfile_write(pid);
  }

  ~SigHandler() {
    pidfile_remove(pid);
    pid = NULL;
  }
};

struct pidfh *SigHandler::pid;
