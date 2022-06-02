all: test_google_photos test_qr

test_google_photos:
	gcc \
	  -I ../google_photos_upload \
	  ./test_google_photos.cpp \
	  ../google_photos_upload/google_photos_upload.a \
	  -l pthread \
	  -l stdc++ \
	  -o ./run_google_photos
	./run_google_photos

test_qr:
	gcc \
	  -D HEIGHT=150 \
	  -D WIDTH=100 \
	  -std=gnu++17 \
	  -I ../e-Paper/RaspberryPi_JetsonNano/c/lib/Config \
	  -I ../e-Paper/RaspberryPi_JetsonNano/c/lib/e-Paper \
	  -I ../QR-Code-generator/cpp \
	  ../QR-Code-generator/cpp/qrcodegen.o \
	  ./test_qr.cpp \
	  -l stdc++ \
	  -o ./run_qr
	./run_qr

clean:
	rm -f run_*
