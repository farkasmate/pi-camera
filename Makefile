all: libcamera-eink

deps: bcm2835-1.71/src/libbcm2835.a libcamera/build/src/libcamera/libcamera.so

install-deps: deps
	$(MAKE) -C bcm2835-1.71 install
	ninja -C libcamera/build install

bcm2835-1.71/src/libbcm2835.a:
	test -d bcm2835-1.71 || wget --quiet http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz -O - | tar xvz
	cd bcm2835-1.71 && ./configure
	$(MAKE) -C bcm2835-1.71

e-Paper/RaspberryPi_JetsonNano/c/bin/EPD_2in13_V2.o:
	$(MAKE) -C e-Paper/RaspberryPi_JetsonNano/c RPI

libcamera/build/src/libcamera/libcamera.so:
	meson -Dprefix=/usr -Dlibdir=lib libcamera/build libcamera
	ninja -C libcamera/build

libcamera-apps/build/core/libcamera_app.so:
	cmake -S libcamera-apps -B libcamera-apps/build -DENABLE_DRM=1 -DENABLE_X11=0 -DENABLE_QT=0 -DENABLE_OPENCV=0 -DENABLE_TFLITE=0
	$(MAKE) -C libcamera-apps/build libcamera_app

libcamera-eink: e-Paper/RaspberryPi_JetsonNano/c/bin/EPD_2in13_V2.o libcamera-apps/build/core/libcamera_app.so
	gcc \
	  -std=gnu++17 \
	  -I ./e-Paper/RaspberryPi_JetsonNano/c/lib/Config \
	  -I ./e-Paper/RaspberryPi_JetsonNano/c/lib/GUI \
	  -I ./e-Paper/RaspberryPi_JetsonNano/c/lib/e-Paper \
	  -I ./libcamera-apps \
	  -I /usr/include/libcamera \
	  libcamera_eink.cpp \
	  ./e-Paper/RaspberryPi_JetsonNano/c/bin/DEV_Config.o \
	  ./e-Paper/RaspberryPi_JetsonNano/c/bin/EPD_2in13_V2.o \
	  ./e-Paper/RaspberryPi_JetsonNano/c/bin/GUI_Paint.o \
	  ./e-Paper/RaspberryPi_JetsonNano/c/bin/font16.o \
	  ./libcamera-apps/build/core/libcamera_app.so \
	  ./libcamera-apps/build/image/libimages.so \
	  ./libcamera-apps/build/post_processing_stages/libpost_processing_stages.so \
	  ./libcamera-apps/build/preview/libpreview.so \
	  -lbcm2835 \
	  -lboost_program_options \
	  -lcamera \
	  -lcamera-base \
	  -lexif \
	  -ljpeg \
	  -lpthread \
	  -lstdc++ \
	  -o libcamera-eink
	strip libcamera-eink

clean:
	$(MAKE) -s -C bcm2835-1.71 clean || true
	$(MAKE) -s -C e-Paper/RaspberryPi_JetsonNano/c clean || true
	$(MAKE) -s -C libcamera-apps/build clean || true
	ninja -C libcamera/build clean || true
	rm -f libcamera-eink
