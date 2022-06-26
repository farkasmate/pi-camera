all: with_deps.o

bcm2835-1.71/src/bcm2835.o:
	test -d bcm2835-1.71 || wget --quiet http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz -O - | tar xvz
	cd bcm2835-1.71 && ./configure
	$(MAKE) -C bcm2835-1.71

e-Paper/RaspberryPi_JetsonNano/c/bin/EPD_2in13_V2.o:
	C_INCLUDE_PATH=../../../bcm2835-1.71/src $(MAKE) -C e-Paper/RaspberryPi_JetsonNano/c RPI_DEV ./bin/GUI_Paint.o ./bin/EPD_2in13_V2.o

eink.o:
	gcc \
	  -I ./e-Paper/RaspberryPi_JetsonNano/c/lib/Config \
	  -I ./e-Paper/RaspberryPi_JetsonNano/c/lib/e-Paper \
	  -c \
	  eink.cpp

frame.o:
	gcc \
	  -I ./e-Paper/RaspberryPi_JetsonNano/c/lib/Config \
	  -I ./e-Paper/RaspberryPi_JetsonNano/c/lib/Fonts \
	  -I ./e-Paper/RaspberryPi_JetsonNano/c/lib/GUI \
	  -c \
	  frame.cpp

with_deps.o: bcm2835-1.71/src/bcm2835.o e-Paper/RaspberryPi_JetsonNano/c/bin/EPD_2in13_V2.o eink.o frame.o
	ld \
	  --relocatable \
	  ./bcm2835-1.71/src/bcm2835.o \
	  ./e-Paper/RaspberryPi_JetsonNano/c/bin/DEV_Config.o \
	  ./e-Paper/RaspberryPi_JetsonNano/c/bin/EPD_2in13_V2.o \
	  ./e-Paper/RaspberryPi_JetsonNano/c/bin/GUI_Paint.o \
	  eink.o \
	  frame.o \
	  -o with_deps.o

rebuild:
	rm -f *.o
	${MAKE} all

clean:
	$(MAKE) -s -C bcm2835-1.71 clean || true
	$(MAKE) -s -C e-Paper/RaspberryPi_JetsonNano/c clean || true
	rm -f *.o
