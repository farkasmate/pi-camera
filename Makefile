all: libcamera-eink

e-Paper/RaspberryPi_JetsonNano/c/bin/EPD_2in13_V2.o:
	$(MAKE) -C e-Paper/RaspberryPi_JetsonNano/c RPI

libcamera-apps/build/core/libcamera_app.so:
	cmake -S libcamera-apps -B libcamera-apps/build -DENABLE_DRM=1 -DENABLE_X11=0 -DENABLE_QT=0 -DENABLE_OPENCV=0 -DENABLE_TFLITE=0
	$(MAKE) -C libcamera-apps/build -j2 libcamera_app

libcamera-eink: e-Paper/RaspberryPi_JetsonNano/c/bin/EPD_2in13_V2.o libcamera-apps/build/core/libcamera_app.so
	gcc \
	  -std=gnu++17 \
	  -I ./e-Paper/RaspberryPi_JetsonNano/c/lib/Config \
	  -I ./e-Paper/RaspberryPi_JetsonNano/c/lib/GUI \
	  -I ./e-Paper/RaspberryPi_JetsonNano/c/lib/e-Paper \
	  -I ./e-Paper/STM32/STM32-F103ZET6/User/GUI \
	  -I ./libcamera-apps \
	  -I /usr/include/libcamera \
	  libcamera_eink.cpp \
	  ./e-Paper/RaspberryPi_JetsonNano/c/bin/DEV_Config.o \
	  ./e-Paper/RaspberryPi_JetsonNano/c/bin/EPD_2in13_V2.o \
	  ./e-Paper/RaspberryPi_JetsonNano/c/bin/GUI_BMPfile.o \
	  ./e-Paper/RaspberryPi_JetsonNano/c/bin/GUI_Paint.o \
	  ./libcamera-apps/build/core/libcamera_app.so \
	  ./libcamera-apps/build/image/libimages.so \
	  ./libcamera-apps/build/post_processing_stages/libpost_processing_stages.so \
	  ./libcamera-apps/build/preview/libpreview.so \
	  -lbcm2835 \
	  -lboost_program_options \
	  -lcamera \
	  -lcamera-base \
	  -ldrm \
	  -lexif \
	  -ljpeg \
	  -lpng \
	  -lpthread \
	  -lstdc++ \
	  -ltiff \
	  -o libcamera-eink
	strip libcamera-eink

clean:
	$(MAKE) -s -C e-Paper/RaspberryPi_JetsonNano/c clean || true
	$(MAKE) -s -C libcamera-apps/build clean || true
	rm -f libcamera-eink
