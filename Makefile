all: pi-camera

eink/with_deps.o:
	$(MAKE) -C eink

google_photos_upload/google_photos_upload.a:
	$(MAKE) -C google_photos_upload

libcamera/build/src/libcamera/libcamera.so:
	meson -Dprefix=/usr -Dlibdir=lib libcamera/build libcamera
	meson compile -C libcamera/build

QR-Code-generator/cpp/qrcodegen.o:
	$(MAKE) -C QR-Code-generator/cpp

pi-camera: eink/with_deps.o libcamera/build/src/libcamera/libcamera.so
	gcc \
	  -std=gnu++17 \
	  -I ./libcamera/build/include \
	  -I ./libcamera/include \
	  main.cpp \
	  ./eink/with_deps.o \
	  -L ./libcamera/build/src/libcamera \
	  -L ./libcamera/build/src/libcamera/base \
	  -l bsd \
	  -l camera \
	  -l camera-base \
	  -l pthread \
	  -l stdc++ \
	  -l turbojpeg \
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
	rm -rf libcamera/build

.PHONY: test
