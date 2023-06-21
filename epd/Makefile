all: libbcm2835.a libepd_2in13_v2.a

libbcm2835.a:
	test -d bcm2835-1.73 || wget --quiet http://www.airspayce.com/mikem/bcm2835/bcm2835-1.73.tar.gz -O - | tar xvz
	cd bcm2835-1.73 && ./configure
	$(MAKE) -C bcm2835-1.73
	mv bcm2835-1.73/src/libbcm2835.a libbcm2835.a

libepd_2in13_v2.a:
	C_INCLUDE_PATH=../../../bcm2835-1.73/src $(MAKE) -C e-Paper/RaspberryPi_JetsonNano/c RPI_DEV ./bin/GUI_Paint.o ./bin/EPD_2in13_V2.o
	ld \
	  --relocatable \
	  ./e-Paper/RaspberryPi_JetsonNano/c/bin/DEV_Config.o \
	  ./e-Paper/RaspberryPi_JetsonNano/c/bin/EPD_2in13_V2.o \
	  ./e-Paper/RaspberryPi_JetsonNano/c/bin/GUI_Paint.o \
	  -o libepd_2in13_v2.a

clean:
	$(MAKE) -s -C bcm2835-1.73 clean || true
	$(MAKE) -s -C e-Paper/RaspberryPi_JetsonNano/c clean || true
	rm -f *.a
