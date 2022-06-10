#include <chrono>

#include "image/image.hpp"

#include "app.hpp"
#include "google_photos_upload/google_photos_upload.h"

#include "viewfinder.cpp"

void PiCameraApp::Capture() {
  OpenCamera();

  bool ready_to_save_still = viewfinder_loop(*this);

  if (!ready_to_save_still)
    throw std::runtime_error("Can't save photo");

  SaveStill();

  // DO NOT call CloseCamera() explicitly
}

PiCameraOptions *PiCameraApp::GetOptions() const { return static_cast<PiCameraOptions *>(options_.get()); }

LibcameraApp::Msg PiCameraApp::GetLatestMsg() {
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

bool PiCameraApp::IsRequestComplete() {
  if (msg.type == MsgType::Quit)
    return false;

  if (msg.type != MsgType::RequestComplete)
    throw std::runtime_error("unrecognised message!");

  return true;
}

void PiCameraApp::SaveStill() {
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

LibcameraApp::Msg PiCameraApp::Wait() {
  msg = LibcameraApp::Wait();
  return msg;
}
