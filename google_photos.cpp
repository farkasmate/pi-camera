#include <algorithm>
#include <filesystem>
#include <sys/stat.h>

#include "eink/eink.hpp"
#include "qrcodegen.hpp"

namespace google_photos {
#include "google_photos_upload/google_photos_upload.h"
}

class GooglePhotos {
private:
  Eink *eink;
  Frame frame;

  void drawQr(const char *text) {
    const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(text, qrcodegen::QrCode::Ecc::LOW);

    int size = 2 * qr.getSize();

    if (size > frame.GetHeight())
      std::cerr << "QR code doesn't fit into frame" << std::endl;

    // NOTE: Draws scaled up to 2x
    for (int y = 0; y < size; y++) {
      for (int x = 0; x < size; x++) {
        if (qr.getModule(x / 2, y / 2))
          frame.SetPixel(x, y);
      }
    }
  }

public:
  GooglePhotos(Eink *eink, std::string config_dir) {
    this->eink = eink;
    google_photos::SetConfig({config_dir.data(), static_cast<std::ptrdiff_t>(config_dir.size())});
  }

  void Authenticate() {
    google_photos::GoString url = google_photos::GetAuthUrl();

    std::cerr << url.p << std::endl;
    drawQr(url.p);
    frame.DrawText(127, 5, "Google Photos");
    frame.DrawText(127, 21, "AUTHENTICATION");
    frame.DrawText(127, 37, "1. Forward 8080");
    frame.DrawText(127, 53, "2. Scan code");
    frame.DrawText(127, 69, "3. Grant access");
    std::cout << frame << std::endl;

    eink->Display(&frame);

    google_photos::WaitForToken(url);

    frame.Clear();
    frame.DrawText(5, 5, "OK");

    eink->Display(&frame);
  }

  void UploadImages(std::string album_name, std::string image_dir) {
    struct stat st = {0};
    std::string archive_dir(image_dir);
    archive_dir.append("/archive");

    if (stat(image_dir.c_str(), &st) == -1) {
      mkdir(image_dir.c_str(), 0700);
    }

    if (stat(archive_dir.c_str(), &st) == -1) {
      mkdir(archive_dir.c_str(), 0700);
    }

    std::filesystem::directory_iterator iterator = std::filesystem::directory_iterator(image_dir);
    int count = std::count_if(begin(iterator), end(iterator), [](auto &entry) { return entry.is_regular_file(); });
    int i = 0;

    for (const std::filesystem::directory_entry &image : std::filesystem::directory_iterator(image_dir)) {
      if (!image.is_regular_file())
        continue;

      i++;
      int percent = i * 100 / count;
      std::string progress_bar = "[]";
      progress_bar.insert(1, 20 - percent / 5, ' ');
      progress_bar.insert(1, percent / 5, '*');

      frame.Clear();
      frame.DrawText(frame.GetWidth() / 2 - 4 - 4 * 8, 21, "UPLOADING");
      frame.DrawText(frame.GetWidth() / 2 - 2 * 8, 37, std::to_string(percent) + "%");
      frame.DrawText(frame.GetWidth() / 2 - 11 * 8, 53, progress_bar);
      frame.DrawText(frame.GetWidth() / 2 - 2 * 8, 69, std::to_string(i) + "/" + std::to_string(count));
      eink->Display(&frame);

      std::string image_path = image.path().string();
      google_photos::UploadImage({album_name.data(), static_cast<std::ptrdiff_t>(album_name.size())},
                                 {image_path.data(), static_cast<std::ptrdiff_t>(image_path.size())});

      std::filesystem::rename(image_path, archive_dir + "/" + image.path().filename().string());
    }
  }
};
