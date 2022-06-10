#include "core/frame_info.hpp"
#include "core/libcamera_app.hpp"

#include "app.hpp"

#include "options.cpp"

class Viewfinder {
private:
  // clang-format off
  static constexpr uint8_t bayer[8][8] = {
      { 0, 32,  8, 40,  2, 34, 10, 42},
      {48, 16, 56, 24, 50, 18, 58, 26},
      {12, 44,  4, 36, 14, 46,  6, 38},
      {60, 28, 52, 20, 62, 30, 54, 22},
      { 3, 35, 11, 43,  1, 33,  9, 41},
      {51, 19, 59, 27, 49, 17, 57, 25},
      {15, 47,  7, 39, 13, 45,  5, 37},
      {63, 31, 55, 23, 61, 29, 53, 21}
  };
  // clang-format on

  PiCameraApp *app;
  std::chrono::milliseconds timeout;

  void Draw(uint8_t *buffer, std::vector<libcamera::Span<uint8_t>> const &mem, StreamInfo const &info) {
    unsigned w = info.width, h = info.height, stride = info.stride;
    uint8_t *Y = (uint8_t *)mem[0].data();

    for (uint8_t j = 0; j < h; j++) {
      for (uint8_t i = 0; i < w; i++) {
        uint8_t yValue = *(Y + j * stride + i);

        // FIXME: draw bitmap in a "window"
        if (yValue > 4 * bayer[j % 8][i % 8])
          buffer[i * LINE_STRIDE + j / 8] |= 1UL << (7 - j % 8);
        else
          buffer[i * LINE_STRIDE + j / 8] &= ~(1UL << (7 - j % 8));
      }
    }
  }

public:
  // FIXME
  Viewfinder(PiCameraApp *app) {
    this->app = app;
    this->timeout = std::chrono::milliseconds(app->GetOptions()->timeout);
  }

  bool Start() {
    app->ConfigureViewfinder();
    app->StartCamera();

    auto start_time = std::chrono::high_resolution_clock::now();

    while (true) {
      LibcameraApp::Msg msg = app->GetLatestMsg();

      if (!app->IsRequestComplete())
        return false;

      if (!app->ViewfinderStream())
        throw std::runtime_error("ViewfinderStream is not available");

      // timeout
      auto now = std::chrono::high_resolution_clock::now();
      if (now - start_time > timeout) {
        std::cerr << "viewfinder timed out" << std::endl;
        app->StopCamera();
        app->Teardown();

        return false;
      }

      // shutter
      if (app->IsShutterPressed()) {
        // FIXME
        std::cerr << "saving photo" << std::endl;
        app->StopCamera();
        app->Teardown();

        return true;
      }

      // draw focus value
      CompletedRequestPtr &completed_request = std::get<CompletedRequestPtr>(msg.payload);
      FrameInfo frame_info(completed_request->metadata);
      libcamera::Stream *stream = app->LoresStream();
      StreamInfo info = app->GetStreamInfo(stream);
      const std::vector<libcamera::Span<uint8_t>> mem = app->Mmap(completed_request->buffers[stream]);

      app->Clear();
      app->DrawFocus(frame_info.focus);
      app->DrawTime((now - start_time) / std::chrono::seconds(1));

      Draw(app->GetBuffer(), mem, info);

      app->Show();
    }
  }
};
