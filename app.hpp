#pragma once

#include "core/libcamera_app.hpp"

#include "options.cpp"

class PiCameraApp : public LibcameraApp {
private:
  const int MSG_TRIES = 10;
  const std::chrono::milliseconds SLOW_MSG_THRESHOLD{10};

  Msg msg = Msg(MsgType::Quit);

public:
  PiCameraApp() : LibcameraApp(std::make_unique<PiCameraOptions>()) {}

  void Capture();

  PiCameraOptions *GetOptions() const;

  Msg GetLatestMsg();

  bool IsRequestComplete();

  void SaveStill();

  Msg Wait();
};
