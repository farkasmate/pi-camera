all: test_google_photos test_icons test_menu test_qr

test_google_photos:
	gcc \
	  -I ../google_photos_upload \
	  ../google_photos_upload/google_photos_upload.a \
	  ./test_google_photos.cpp \
	  -l pthread \
	  -l stdc++ \
	  -o ./run_google_photos
	./run_google_photos

test_icons:
	gcc \
	  -std=gnu++17 \
	  ../eink/with_deps.o \
	  ./test_icons.cpp \
	  -l bsd \
	  -l pthread \
	  -l stdc++ \
	  -o ./run_icons
	./run_icons

test_menu:
	gcc \
	  -std=gnu++17 \
	  ../eink/with_deps.o \
	  ../menu/menu.o \
	  ./test_menu.cpp \
	  -l bsd \
	  -l pthread \
	  -l stdc++ \
	  -o ./run_menu
	./run_menu --help

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
