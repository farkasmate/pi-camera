#include <iostream>
#include <fstream>

#include "google_photos_upload.h"

int main(int argc, char *argv[]) {
  char album_name[] = "pi-camera-test";
  char image_path[] = "./test.jpg";

  upload_test_image(album_name, image_path);

  return 0;
}
