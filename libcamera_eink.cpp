/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) 2020, Raspberry Pi (Trading) Ltd.
 *
 * Based on:
 *   libcamera_hello.cpp - libcamera "hello world" app.
 *   libcamera_still.cpp - libcamera stills capture app.
 */

#include <chrono>
#include <csignal>

#include <bsd/libutil.h>

#include "eink.cpp"
#include "libcamera_eink_app.cpp"
#include "qr.cpp"
#include "viewfinder.cpp"

#include "qrcodegen.hpp"

// FIXME: remove globals
struct pidfh *pid;

// The main event loop for the application.

static void event_loop(LibcameraEinkApp &app) {
  app.OpenCamera();

  bool ready_to_save_still = viewfinder_loop(app);

  if (!ready_to_save_still)
    throw std::runtime_error("Can't save photo");

  app.SaveStill();

  const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText("https://indeed.sch.bme.hu/web/latest.jpg", qrcodegen::QrCode::Ecc::HIGH);
  draw_qr(Image, qr);
  eink_display_partial();

  // DO NOT call app.CloseCamera() explicitly
}

static void sigint_handler(int signo) {
  eink_close();
  pidfile_remove(pid);

  exit(1);
}

static void sigusr1_handler(int signo) { shutter = true; }

static void create_pid_file() {

  pid = pidfile_open("/var/run/libcamera_eink.pid", 0600, NULL);

  if (pid == NULL && errno == EEXIST) {
    std::cerr << "can't lock PID file" << std::endl;

    exit(2);
  }

  pidfile_write(pid);
}

int main(int argc, char *argv[]) {
  std::signal(SIGINT, sigint_handler);
  std::signal(SIGUSR1, sigusr1_handler);

  create_pid_file();

  eink_open();

  try {
    LibcameraEinkApp app;
    StillOptions *options = app.GetOptions();

    if (options->Parse(argc, argv)) {
      // hardcode options
      // FIXME: rotation = 180
      options->nopreview = true;
      options->lores_width = 160;
      options->lores_height = 122;
      options->encoding = "yuv420";

      if (options->verbose)
        options->Print();

      event_loop(app);
    }
  } catch (std::exception const &e) {
    std::cerr << "ERROR: *** " << e.what() << " ***" << std::endl;
    eink_close();

    return -1;
  }

  eink_close();
  pidfile_remove(pid);

  return 0;
}
