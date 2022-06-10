#include "google_photos_upload/google_photos_upload.h"

class GooglePhotos {
public:
  GooglePhotos(char *config_dir) { google_photos_set_config(config_dir); }

  void Authenticate() { google_photos_authenticate(); }

  void UploadImage(char *album_name, char *image_path) { google_photos_upload_image(album_name, image_path); }
};
