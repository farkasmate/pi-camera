all: eink libcamera-eink

eink:
	gcc \
	  -I ./e-Paper/RaspberryPi_JetsonNano/c/lib/Config \
	  -I ./e-Paper/RaspberryPi_JetsonNano/c/lib/GUI \
	  -I ./e-Paper/RaspberryPi_JetsonNano/c/lib/e-Paper \
	  -I ./e-Paper/STM32/STM32-F103ZET6/User/GUI \
	  ./e-Paper/RaspberryPi_JetsonNano/c/lib/Config/DEV_Config.c \
	  ./e-Paper/RaspberryPi_JetsonNano/c/lib/GUI/GUI_BMPfile.c \
	  ./e-Paper/RaspberryPi_JetsonNano/c/lib/GUI/GUI_Paint.c \
	  ./e-Paper/RaspberryPi_JetsonNano/c/lib/e-Paper/EPD_2in13_V2.c \
	  eink.c \
	  -lbcm2835 \
	  -lm \
	  -D RPI \
	  -D USE_BCM2835_LIB \
	  -static \
	  -o eink
	strip eink

libcamera-apps/build/core/libcamera_app.so:
	cd libcamera-apps && \
	mkdir -p build && \
	cd build && \
	cmake .. -DENABLE_DRM=1 -DENABLE_X11=0 -DENABLE_QT=0 -DENABLE_OPENCV=0 -DENABLE_TFLITE=0 && \
	make -j2 libcamera_app

libcamera-eink: libcamera-apps/build/core/libcamera_app.so
	gcc \
	  -std=gnu++17 \
	  -I ./libcamera-apps \
	  -I /usr/include/libcamera \
	  libcamera_eink.cpp \
	  ./libcamera-apps/build/core/libcamera_app.so \
	  ./libcamera-apps/build/image/libimages.so \
	  ./libcamera-apps/build/post_processing_stages/libpost_processing_stages.so \
	  ./libcamera-apps/build/preview/libpreview.so \
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
