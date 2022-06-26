#include <iostream>
#include <fstream>

#include "../google_photos.cpp"

int main(int argc, char *argv[]) {
  char album_name[] = "pi-camera-test";
  char config_dir[] = "./.pi-camera";
  char image_path[] = "./test.jpg";

  GooglePhotos gphotos = GooglePhotos(config_dir);

  gphotos.UploadImage(album_name, image_path);

  return 0;
}
