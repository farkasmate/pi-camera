#pragma once

#include "core/libcamera_app.hpp"

#include "dimensions.hpp"

#include "eink.cpp"
#include "options.cpp"

class PiCameraApp : public LibcameraApp {
private:
  const int MSG_TRIES = 10;
  const std::chrono::milliseconds SLOW_MSG_THRESHOLD{10};

  int bufferSize = ((WIDTH % 8 == 0) ? (WIDTH / 8) : (WIDTH / 8 + 1)) * HEIGHT;
  uint8_t *buffer = NULL;

  Eink *eink;
  Msg msg = Msg(MsgType::Quit);

  bool is_query_only = false;
  bool is_shutter_pressed = false;

public:
  PiCameraApp(int argc, char *argv[]) : LibcameraApp(std::make_unique<PiCameraOptions>()) {
    if ((buffer = (uint8_t *)malloc(bufferSize)) == NULL)
      throw std::runtime_error("Failed to allocate buffer");

    is_query_only = !options_->Parse(argc, argv);

    if (!is_query_only)
      eink = new Eink(buffer, GetOptions()->headless);
  }

  ~PiCameraApp() {
    if (buffer != NULL) {
      free(buffer);
      buffer = NULL;
    }
  }

  void Capture();

  uint8_t *GetBuffer() { return buffer; }

  PiCameraOptions *GetOptions() const { return static_cast<PiCameraOptions *>(options_.get()); }

  Msg GetLatestMsg();

  bool IsRequestComplete();

  bool IsQueryOnly() { return is_query_only; }

  bool IsShutterPressed() { return is_shutter_pressed; }

  void PressShutter() { is_shutter_pressed = true; }

  void SaveStill();

  Msg Wait();

  // eink
  void Clear() { eink->Clear(); }
  void Show() { eink->DisplayPartial(); }
  void DrawFocus(int focus) { eink->DrawFocus(focus); }
  void DrawTime(int time) { eink->DrawTime(time); }
};
