#include "eink.cpp"

#include "core/frame_info.hpp"
#include "core/libcamera_app.hpp"

#include "app.cpp"
#include "options.cpp"

// FIXME: remove globals
bool shutter = false;

static void draw_viewfinder(UBYTE *image, std::vector<libcamera::Span<uint8_t>> const &mem, StreamInfo const &info) {
  unsigned w = info.width, h = info.height, stride = info.stride;
  uint8_t *Y = (uint8_t *)mem[0].data();

  for (uint8_t j = 0; j < h; j++) {
    for (uint8_t i = 0; i < w; i++) {
      uint8_t yValue = *(Y + j * stride + i);

      // FIXME: draw bitmap in a "window"
      if (yValue > 4 * bayer[j % 8][i % 8])
        image[i * LINE_STRIDE + j / 8] |= 1UL << (7 - j % 8);
      else
        image[i * LINE_STRIDE + j / 8] &= ~(1UL << (7 - j % 8));
    }
  }
}

static bool viewfinder_loop(PiCameraApp &app) {
  Options const *options = app.GetOptions();

  app.ConfigureViewfinder();
  app.StartCamera();

  auto start_time = std::chrono::high_resolution_clock::now();

  eink_display_base();

  while (true) {
    LibcameraApp::Msg msg = app.GetLatestMsg();

    if (!app.IsRequestComplete())
      return false;

    if (!app.ViewfinderStream())
      throw std::runtime_error("ViewfinderStream is not available");

    // timeout
    auto now = std::chrono::high_resolution_clock::now();
    if (options->timeout && now - start_time > std::chrono::milliseconds(options->timeout)) {
      std::cerr << "viewfinder timed out" << std::endl;
      app.StopCamera();
      app.Teardown();

      return false;
    }

    // shutter
    if (shutter) {
      std::cerr << "saving photo" << std::endl;
      app.StopCamera();
      app.Teardown();

      return true;
    }

    // draw focus value
    CompletedRequestPtr &completed_request = std::get<CompletedRequestPtr>(msg.payload);
    FrameInfo frame_info(completed_request->metadata);
    libcamera::Stream *stream = app.LoresStream();
    StreamInfo info = app.GetStreamInfo(stream);
    const std::vector<libcamera::Span<uint8_t>> mem = app.Mmap(completed_request->buffers[stream]);

    eink_clear();
    eink_draw_focus(frame_info.focus);
    eink_draw_time((now - start_time) / std::chrono::seconds(1));

    draw_viewfinder(Image, mem, info);

    eink_display_partial();
  }
}
