#include <fstream>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <turbojpeg.h>
#include <unistd.h>

#include "app.hpp"

PiCameraApp::PiCameraApp(Eink *eink) {
  this->eink = eink;
  save_thread = NULL;
}

void PiCameraApp::configureCamera() {
  std::unique_ptr<CameraConfiguration> config = camera->generateConfiguration({StreamRole::Viewfinder, StreamRole::StillCapture});
  streams[VIEWFINDER].config = &config->at(VIEWFINDER);
  streams[STILL].config = &config->at(STILL);

  config->transform = Transform::Rot180;

  std::cerr << "Default still configuration is: " << streams[STILL].config->toString() << std::endl;
  std::cerr << "Default viewfinder configuration is: " << streams[VIEWFINDER].config->toString() << std::endl;

  streams[VIEWFINDER].config->bufferCount = 1;
  streams[VIEWFINDER].config->pixelFormat = libcamera::formats::YUV420;
  streams[VIEWFINDER].config->size.height = 122;
  streams[VIEWFINDER].config->size.width = 162;

  streams[STILL].config->bufferCount = 1;
  streams[STILL].config->pixelFormat = libcamera::formats::BGR888;

  config->validate();
  std::cerr << "Validated still configuration is: " << streams[STILL].config->toString() << std::endl;
  std::cerr << "Validated viewfinder configuration is: " << streams[VIEWFINDER].config->toString() << std::endl;

  camera->configure(config.get());

  camera->requestCompleted.connect(requestComplete);
}

void PiCameraApp::requestComplete(Request *request) {
  std::cerr << "Processing request... " << request->sequence() << std::endl;

  if (request->status() == Request::RequestCancelled)
    return;

  ControlList controls = request->metadata();
  std::optional<int> focus = controls.get(libcamera::controls::FocusFoM);

  drawViewfinder(focus.has_value() ? focus.value() / 100 : 0);

  if (is_shutter_pressed) {
    save_thread = new std::thread(&PiCameraApp::saveJpeg);
    return;
  }

  request->reuse(Request::ReuseFlag::ReuseBuffers);
  camera->queueRequest(request);
}

void PiCameraApp::drawViewfinder(int focus) {
  std::cerr << "drawing on eink" << std::endl;

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

  StreamConfiguration *vf_config = streams[VIEWFINDER].config;
  uint8_t *Y = streams[VIEWFINDER].data;

  frame.Clear();
  frame.DrawText(170, 8, "F: " + std::to_string(focus));

  for (int y = 0; y < vf_config->size.height; y++) {
    for (int x = 0; x < vf_config->size.width; x++) {
      uint8_t yValue = *(Y + y * vf_config->stride + x);

      if (yValue < 4 * bayer[x % 8][y % 8])
        frame.SetPixel(x, y);
    }
  }

  eink->Display(&frame);
}

void PiCameraApp::saveJpeg() {
  std::cerr << "saving JPEG" << std::endl;

  FILE *jpeg_file;
  tjhandle turbo_jpeg;
  unsigned char *jpeg_buffer = NULL;
  unsigned long jpeg_size = 0;
  struct stat st = {0};

  StreamConfiguration *still_config = streams[STILL].config;

  if ((turbo_jpeg = tjInitCompress()) == NULL)
    std::cerr << tjGetErrorStr2(turbo_jpeg) << std::endl;

  // NOTE: slow
  tjCompress2(turbo_jpeg, streams[STILL].data, still_config->size.width, still_config->stride, still_config->size.height, TJPF_RGB, &jpeg_buffer, &jpeg_size,
              TJSAMP_444, 95, 0);

  if (stat("./DCIM", &st) == -1) {
    mkdir("./DCIM", 0700);
  }

  std::string filename = "./DCIM/pi_camera_";
  filename.append(std::to_string(std::time(0)));
  filename.append(".jpg");
  jpeg_file = fopen(filename.c_str(), "wb");
  fwrite(jpeg_buffer, 1, jpeg_size, jpeg_file);

  fclose(jpeg_file);
}

void PiCameraApp::Start() {
  eink->Start();

  camera_manager = std::make_unique<CameraManager>();
  camera_manager->start();

  if (camera_manager->cameras().empty())
    throw std::runtime_error("No cameras");

  camera = camera_manager->cameras()[0];
  allocator = new FrameBufferAllocator(camera);

  camera->acquire();

  if (camera->streams().size() < 2)
    throw std::runtime_error("Not enough streams");

  configureCamera();

  prepared_request = camera->createRequest();

  for (auto &[key, metadata] : streams) {
    if (allocator->allocate(metadata.config->stream()) < 0)
      throw std::runtime_error("Can't allocate buffers");

    // NOTE: Single buffers only, RGB is single-plane, YUV420 has 3 planes, but we only need the first for monochrome image
    const std::unique_ptr<FrameBuffer> &buffer = allocator->buffers(metadata.config->stream()).front();
    const FrameBuffer::Plane plane = buffer->planes().front();

    void *memory = mmap(NULL, plane.length, PROT_READ | PROT_WRITE, MAP_SHARED, plane.fd.get(), 0);
    metadata.data = static_cast<uint8_t *>(memory);
    metadata.size = plane.length;

    prepared_request->addBuffer(metadata.config->stream(), buffer.get());
  }

  camera->start();
  camera->queueRequest(prepared_request.get());
}

void PiCameraApp::Stop() {
  std::thread *eink_thread = new std::thread(&Eink::Stop, eink);

  if (save_thread != NULL)
    save_thread->join();

  eink_thread->join();

  camera->stop();

  for (auto &[key, metadata] : streams) {
    munmap(metadata.data, metadata.size);
    allocator->free(metadata.config->stream());
  }

  delete allocator;
  camera->release();
  camera.reset();
  camera_manager->stop();
}
