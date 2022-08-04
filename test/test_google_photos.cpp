#include <iostream>
#include <fstream>

#include "../google_photos.cpp"

int main(int argc, char *argv[]) {
  char album_name[] = "pi-camera-test";
  char config_dir[] = "./.pi-camera";
  char image_path[] = "./test.jpg";

  Eink eink;
  eink.Start();

  GooglePhotos gphotos = GooglePhotos(&eink, config_dir);

  gphotos.Authenticate();

  gphotos.UploadImage(album_name, image_path);

  eink.Stop();

  return 0;
}
