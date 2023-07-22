all: libcamera_c_api.a

libcamera_c_api.a:
	gcc \
	  -std=c++17 \
	  -I /usr/include/libcamera \
	  -c \
	  *.cpp
	ar rc $@ *.o

clean:
	rm -f *.a *.o
