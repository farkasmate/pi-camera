#include "core/frame_info.hpp"
#include "core/libcamera_app.hpp"

#include "eink/frame.hpp"

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
  Frame *frame;
  std::chrono::milliseconds timeout;

  void draw(std::vector<libcamera::Span<uint8_t>> const &mem, StreamInfo const &info) {
    unsigned w = info.width, h = info.height, stride = info.stride;
    uint8_t *Y = (uint8_t *)mem[0].data();

    for (uint8_t j = 0; j < h; j++) {
      for (uint8_t i = 0; i < w; i++) {
        uint8_t yValue = *(Y + j * stride + i);

        if (yValue < 4 * bayer[j % 8][i % 8])
          frame->SetPixel(i, j);
      }
    }
  }

public:
  Viewfinder(PiCameraApp *app) {
    this->app = app;
    this->frame = app->GetFrame();
    this->timeout = std::chrono::milliseconds(app->GetOptions()->timeout);
  }

  bool Start() {
    app->OpenCamera();
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

      frame->Clear();
      frame->DrawText(160, 0, "FOCUS: " + std::to_string((int)frame_info.focus / 100));
      frame->DrawText(160, 20, "TIME: " + std::to_string((now - start_time) / std::chrono::seconds(1)));

      draw(mem, info);

      app->Show();
    }
  }
};
