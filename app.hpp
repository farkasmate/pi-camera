#pragma once

#include <libcamera/libcamera.h>

#include "eink/eink.hpp"

using namespace libcamera;

class PiCameraApp {
private:
  enum PiCameraStream {
    VIEWFINDER,
    STILL,
  };

  struct StreamMetadata {
    StreamConfiguration *config;
    uint8_t *data;
    int size;
  };

  static Eink *eink;
  static Frame frame;

  static bool is_shutter_pressed;
  static std::thread *save_thread;

  static std::shared_ptr<Camera> camera;
  static std::map<PiCameraStream, StreamMetadata> streams;

  std::unique_ptr<CameraManager> camera_manager;
  FrameBufferAllocator *allocator;
  std::shared_ptr<libcamera::Request> prepared_request;

  void configureCamera();

public:
  PiCameraApp(Eink *eink);

  static void requestComplete(Request *request);
  static void drawViewfinder(int focus);
  static void saveJpeg();

  bool IsShutterPressed() { return (is_shutter_pressed && (save_thread != NULL)); }
  void pressShutter() { is_shutter_pressed = true; }

  void Start();
  void Stop();
};

Eink *PiCameraApp::eink;
Frame PiCameraApp::frame;

bool PiCameraApp::is_shutter_pressed;
std::thread *PiCameraApp::save_thread;

std::shared_ptr<Camera> PiCameraApp::camera;
std::map<PiCameraApp::PiCameraStream, PiCameraApp::StreamMetadata> PiCameraApp::streams;
