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

#include "qrcodegen.hpp"

#include "app.cpp"
#include "eink.cpp"
#include "google_photos.cpp"
#include "qr.cpp"

// FIXME: remove globals
struct pidfh *pid;
PiCameraApp *app;

static void sigint_handler(int signo) {
  pidfile_remove(pid);

  exit(1);
}

static void sigusr1_handler(int signo) {
  if (app == NULL)
    return;

  app->PressShutter();
}

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
    app = new PiCameraApp(argc, argv);
    PiCameraOptions *options = app->GetOptions();

    // FIXME
    char config_dir[] = "./.pi-camera";
    GooglePhotos gphotos = GooglePhotos(config_dir);

    if (app->IsQueryOnly())
      return -1;

    if (options->verbose)
      options->Print();

    if (options->auth)
      gphotos.Authenticate();

    if (options->capture)
      app->Capture();

    const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText("https://indeed.sch.bme.hu/web/latest.jpg", qrcodegen::QrCode::Ecc::HIGH);
    draw_qr(app->GetBuffer(), qr);
    app->Show();
  } catch (std::exception const &e) {
    std::cerr << "ERROR: *** " << e.what() << " ***" << std::endl;

    return -2;
  }

  pidfile_remove(pid);

  return 0;
}
