all: pi-camera

eink/with_deps.o:
	$(MAKE) -C eink

google_photos_upload/google_photos_upload.a:
	$(MAKE) -C google_photos_upload

libcamera/build/src/libcamera/libcamera.so:
	meson -Dprefix=/usr -Dlibdir=lib libcamera/build libcamera
	meson compile -C libcamera/build

libcamera-apps/build/core/libcamera_app.so:
	cmake -S libcamera-apps -B libcamera-apps/build -DENABLE_DRM=1 -DENABLE_X11=0 -DENABLE_QT=0 -DENABLE_OPENCV=0 -DENABLE_TFLITE=0
	$(MAKE) -C libcamera-apps/build libcamera_app

QR-Code-generator/cpp/qrcodegen.o:
	$(MAKE) -C QR-Code-generator/cpp

pi-camera: eink/with_deps.o google_photos_upload/google_photos_upload.a libcamera-apps/build/core/libcamera_app.so QR-Code-generator/cpp/qrcodegen.o
	gcc \
	  -std=gnu++17 \
	  -I ./QR-Code-generator/cpp \
	  -I ./libcamera-apps \
	  -I ./libcamera/build/include \
	  -I ./libcamera/include \
	  main.cpp \
	  ./QR-Code-generator/cpp/qrcodegen.o \
	  ./eink/with_deps.o \
	  ./google_photos_upload/google_photos_upload.a \
	  ./libcamera-apps/build/core/libcamera_app.so \
	  ./libcamera-apps/build/image/libimages.so \
	  -L ./libcamera/build/src/libcamera \
	  -L ./libcamera/build/src/libcamera/base \
	  -l boost_program_options \
	  -l bsd \
	  -l camera \
	  -l camera-base \
	  -l exif \
	  -l jpeg \
	  -l pthread \
	  -l stdc++ \
	  -o pi-camera
	strip pi-camera

test: pi-camera
	$(MAKE) -C test clean all

test-menu: install-deps
	${MAKE} -C menu

rebuild:
	$(MAKE) -C eink rebuild
	rm -f pi-camera
	$(MAKE) pi-camera

clean:
	@echo -n "Are you sure to clean the whole project? [y/N] " && read ans && [ $${ans:-N} = y ]
	$(MAKE) -C eink clean
	rm -f pi-camera
	rm -rf libcamera-apps/build
	rm -rf libcamera/build

.PHONY: test
