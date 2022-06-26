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
	  -std=gnu++17 \
	  -I ../QR-Code-generator/cpp \
	  ../QR-Code-generator/cpp/qrcodegen.o \
	  ../eink/with_deps.o \
	  ./test_qr.cpp \
	  -l pthread \
	  -l stdc++ \
	  -o ./run_qr
	./run_qr

clean:
	rm -f run_*
