#include <iostream>
#include <fstream>

#include "../google_photos.cpp"

int main(int argc, char *argv[]) {
  Eink eink;
  eink.Start();

  GooglePhotos gphotos = GooglePhotos(&eink, "./.pi-camera");

  //gphotos.Authenticate();

  gphotos.UploadImages("pi-camera-test", "./DCIM");

  eink.Stop();

  return 0;
}
