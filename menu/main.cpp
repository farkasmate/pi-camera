#include <iostream>
#include <thread>

#include <unistd.h>

#include "sig_handler.cpp"

std::string menu[] = { "capture", "upload", "sync", "show" };
int selection = 0;

std::thread::native_handle_type draw_thread_handle;

void sigusr1_handler(int signo) { selection = ++selection % (sizeof(menu)/sizeof(*menu)) ; }

void sigusr2_handler(int signo) { pthread_cancel(draw_thread_handle); }

void draw_menu() {
  while (true) {
    std::cout << "+----------------+" << std::endl;

    for (int i = 0; i < (sizeof(menu)/sizeof(*menu)); i++) {
      if (i == selection)
        std::cout << "  ";

      std::cout << menu[i] << std::endl;
    }

    std::cout << "+----------------+" << std::endl;
    std::cout << std::endl;

    sleep(1);
  }
}

int main(int argc, char *argv[]) {
  SigHandler::hook("/tmp/menu.pid", &sigusr1_handler, &sigusr2_handler);

  std::thread draw_thread (draw_menu);
  draw_thread_handle = draw_thread.native_handle();

  draw_thread.join();

  std::cout << "Running selected menu item: " << menu[selection] << std::endl;
  sleep(2);

  return 0;
}
