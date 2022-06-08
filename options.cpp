#pragma once

#include "core/still_options.hpp"

struct PiCameraOptions : public StillOptions {
  bool auth;
  bool capture;
  bool headless;
  bool sync;
  bool upload;

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

  virtual bool Parse(int argc, char *argv[]) override {
    if (StillOptions::Parse(argc, argv) == false)
      return false;

    // hardcode options
    // FIXME: rotation = 180
    this->nopreview = true;
    this->lores_width = 160;
    this->lores_height = 122;
    this->encoding = "yuv420";

    return true;
  }

  virtual void Print() const override {
    StillOptions::Print();

    std::cerr << "    auth: " << (auth ? "true" : "false") << std::endl;
    std::cerr << "    capture: " << (capture ? "true" : "false") << std::endl;
    std::cerr << "    headless: " << (headless ? "true" : "false") << std::endl;
    std::cerr << "    sync: " << (sync ? "true" : "false") << std::endl;
    std::cerr << "    upload: " << (upload ? "true" : "false") << std::endl;
  }
};
