all:
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

