all: pi-camera

eink/with_deps.o:
	$(MAKE) -C eink

google_photos_upload/google_photos_upload.a:
	$(MAKE) -C google_photos_upload

menu/menu.o:
	${MAKE} -C menu

QR-Code-generator/cpp/qrcodegen.o:
	$(MAKE) -C QR-Code-generator/cpp

pi-camera: eink/with_deps.o google_photos_upload/google_photos_upload.a menu/menu.o
	gcc \
	  -std=gnu++17 \
	  -I ./QR-Code-generator/cpp \
          -I /usr/include/libcamera \
	  main.cpp \
	  ./eink/with_deps.o \
	  ./google_photos_upload/google_photos_upload.a \
	  ./menu/menu.o \
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

rebuild:
	$(MAKE) -C eink rebuild
	$(MAKE) -C menu rebuild
	rm -f pi-camera
	$(MAKE) pi-camera

clean:
	@echo -n "Are you sure to clean the whole project? [y/N] " && read ans && [ $${ans:-N} = y ]
	$(MAKE) -C eink clean
	rm -f pi-camera

.PHONY: test
