#pragma once

#include "core/still_options.hpp"

struct PiCameraOptions : public StillOptions {
  PiCameraOptions() : StillOptions() {
    using namespace boost::program_options;

    // clang-format off
    options_.add_options()
      ("auth", value<bool>(&auth)->default_value(false)->implicit_value(true), "Authenticate to Google Photos")
      ("capture", value<bool>(&capture)->default_value(true)->implicit_value(true), "Capture image")
      ("headless", value<bool>(&headless)->default_value(false)->implicit_value(true), "Run without eink display")
      ("sync", value<bool>(&sync)->default_value(false)->implicit_value(true), "Sync cached images to Google Photos")
      ("upload", value<bool>(&upload)->default_value(false)->implicit_value(true), "Upload captured image to Google Photos")
      ;
    // clang-format on
  }

  bool auth;
  bool capture;
  bool headless;
  bool sync;
  bool upload;

  virtual void Print() const override {
    StillOptions::Print();

    std::cerr << "    auth: " << auth << std::endl;
    std::cerr << "    capture: " << capture << std::endl;
    std::cerr << "    headless: " << headless << std::endl;
    std::cerr << "    sync: " << sync << std::endl;
    std::cerr << "    upload: " << upload << std::endl;
  }
};
