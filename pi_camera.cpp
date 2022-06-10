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

#include "app.cpp"
#include "eink.cpp"
#include "qr.cpp"

#include "qrcodegen.hpp"

// FIXME: remove globals
struct pidfh *pid;

static void sigint_handler(int signo) {
  eink_close();
  pidfile_remove(pid);

  exit(1);
}

static void sigusr1_handler(int signo) { shutter = true; }

static void create_pid_file() {

  pid = pidfile_open("/var/run/pi_camera.pid", 0600, NULL);

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

  try {
    PiCameraApp app;
    PiCameraOptions *options = app.GetOptions();

    if (options->Parse(argc, argv)) {
      if (options->verbose)
        options->Print();

      if (!options->headless)
        eink_open();

      if (options->auth)
        authenticate();

      if (options->capture)
        app.Capture();
    }
  } catch (std::exception const &e) {
    std::cerr << "ERROR: *** " << e.what() << " ***" << std::endl;
    eink_close();

    return -1;
  }

  const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText("https://indeed.sch.bme.hu/web/latest.jpg", qrcodegen::QrCode::Ecc::HIGH);
  draw_qr(Image, qr);
  eink_display_partial();

  eink_close();
  pidfile_remove(pid);

  return 0;
}
