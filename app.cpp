#pragma once

#include <chrono>

#include "core/libcamera_app.hpp"
#include "image/image.hpp"

#include "options.cpp"

class PiCameraApp : public LibcameraApp {
private:
  const int MSG_TRIES = 10;
  const std::chrono::milliseconds SLOW_MSG_THRESHOLD{10};

  Msg msg = Msg(MsgType::Quit);

public:
  PiCameraApp() : LibcameraApp(std::make_unique<PiCameraOptions>()) {}

  PiCameraOptions *GetOptions() const { return static_cast<PiCameraOptions *>(options_.get()); }

  Msg GetLatestMsg() {
    int tries = MSG_TRIES;
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    std::chrono::high_resolution_clock::time_point stop = start;
    std::chrono::microseconds duration{0};

    while (duration < SLOW_MSG_THRESHOLD && tries-- > 0) {
      start = std::chrono::high_resolution_clock::now();
      msg = LibcameraApp::Wait();
      stop = std::chrono::high_resolution_clock::now();
      duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    }

    return msg;
  }

  bool IsRequestComplete() {
    if (msg.type == MsgType::Quit)
      return false;

    if (msg.type != MsgType::RequestComplete)
      throw std::runtime_error("unrecognised message!");

    return true;
  }

  void SaveStill() {
    ConfigureStill(FLAG_STILL_NONE);
    StartCamera();

    Wait();

    if (!IsRequestComplete())
      return;

    StopCamera();
    std::cerr << "Still capture image received" << std::endl;

    if (options_->output.empty()) {
      std::cerr << "No output specified, skip saving still" << std::endl;
      return;
    }

    std::string filename = options_->output;

    libcamera::Stream *stream = StillStream();
    CompletedRequestPtr &payload = std::get<CompletedRequestPtr>(msg.payload);
    StreamInfo info = GetStreamInfo(stream);
    const std::vector<libcamera::Span<uint8_t>> mem = Mmap(payload->buffers[stream]);
    jpeg_save(mem, info, payload->metadata, filename, CameraId(), GetOptions());

    return;
  }

  Msg Wait() {
    msg = LibcameraApp::Wait();
    return msg;
  }
};
