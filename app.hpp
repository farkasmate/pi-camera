#pragma once

#include "core/libcamera_app.hpp"

#include "eink/eink.hpp"
#include "eink/frame.hpp"

#include "options.cpp"

class PiCameraApp : public LibcameraApp {
private:
  const int MSG_TRIES = 10;
  const std::chrono::milliseconds SLOW_MSG_THRESHOLD{10};

  Eink *eink;
  Msg msg = Msg(MsgType::Quit);

  bool is_query_only = false;
  bool is_shutter_pressed = false;

public:
  PiCameraApp(int argc, char *argv[]) : LibcameraApp(std::make_unique<PiCameraOptions>()) {
    is_query_only = !options_->Parse(argc, argv);

    if (!is_query_only)
      eink = new Eink(GetOptions()->headless, true);
  }

  void Capture();

  Frame* GetFrame() { return eink->GetFrame(); }

  PiCameraOptions* GetOptions() const { return static_cast<PiCameraOptions *>(options_.get()); }

  Msg GetLatestMsg();

  bool IsRequestComplete();

  bool IsQueryOnly() { return is_query_only; }

  bool IsShutterPressed() { return is_shutter_pressed; }

  void PressShutter() { is_shutter_pressed = true; }

  void SaveStill();

  void Show(bool wait = false);

  Msg Wait();
};
