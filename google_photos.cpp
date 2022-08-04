#include "eink/eink.hpp"
#include "google_photos_upload/google_photos_upload.h"
#include "qrcodegen.hpp"

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
  GooglePhotos(Eink *eink, char *config_dir) {
    this->eink = eink;
    google_photos_set_config(config_dir);
  }

  void Authenticate() {
    char *url = NULL;
    url = google_photos_get_auth_url();

    std::cout << url << std::endl;
    drawQr(url);
    frame.DrawText(127, 5, "Google Photos");
    frame.DrawText(127, 21, "AUTHENTICATION");
    frame.DrawText(127, 37, "1. Forward 8080");
    frame.DrawText(127, 53, "2. Scan code");
    frame.DrawText(127, 69, "3. Grant access");
    std::cout << frame << std::endl;

    eink->Display(&frame);

    google_photos_wait_for_token(url);

    frame.Clear();
    frame.DrawText(5, 5, "OK");

    eink->Display(&frame);

    free(url);
    url = NULL;
  }

  void UploadImage(char *album_name, char *image_path) { google_photos_upload_image(album_name, image_path); }
};
