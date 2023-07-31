# syntax=docker/dockerfile:1-labs
FROM matefarkas/crystal:1.8.2-alpine AS BUILDER

ENV CRYSTAL_LIBRARY_PATH=/build/lib

WORKDIR /build/

COPY shard.yml shard.lock .
COPY spec/ spec/
COPY src/ src/

ENV CRYSTAL_CACHE_DIR=/root/.cache/crystal

RUN --mount=type=cache,target=/root/.cache/crystal \
  shards build \
  --cross-compile \
  --target=arm-linux-gnueabihf \
  | tee /tmp/build.log \
  && grep '^cc' /tmp/build.log > link.sh

FROM raspbian/stretch:latest AS LIBBCM2835

WORKDIR /build/

RUN echo 'deb http://raspbian.raspberrypi.org/raspbian/ bullseye main contrib non-free rpi' > /etc/apt/sources.list \
  && echo 'deb http://archive.raspberrypi.org/debian/ bullseye main' >> /etc/apt/sources.list \
  && apt-get update \
  && apt-get install -y \
    build-essential \
    wget \
  && rm -rf /var/lib/apt/lists/*

RUN wget --quiet http://www.airspayce.com/mikem/bcm2835/bcm2835-1.73.tar.gz -O - | tar xvz \
  && ./bcm2835-1.73/configure \
  && make

FROM raspbian/stretch:latest AS LIBCAMERA_C_API

ARG LIBCAMERA_VERSION

WORKDIR /build/

RUN echo 'deb http://raspbian.raspberrypi.org/raspbian/ bullseye main contrib non-free rpi' > /etc/apt/sources.list \
  && echo 'deb http://archive.raspberrypi.org/debian/ bullseye main' >> /etc/apt/sources.list \
  && apt-get update \
  && apt-get install -y \
    build-essential \
    libcamera-dev \
  && rm -rf /var/lib/apt/lists/* \
  && mkdir /tmp/libcamera \
  && wget -P /tmp/libcamera  http://archive.raspberrypi.org/debian/pool/main/libc/libcamera/libcamera-dev_${LIBCAMERA_VERSION}_armhf.deb \
  && wget -P /tmp/libcamera  http://archive.raspberrypi.org/debian/pool/main/libc/libcamera/libcamera0_${LIBCAMERA_VERSION}_armhf.deb \
  && dpkg -iR /tmp/libcamera \
  && rm -rf /tmp/libcamera

COPY libcamera-c_api/*.cpp .
COPY libcamera-c_api/*.h .
COPY libcamera-c_api/Makefile .

RUN make

FROM raspbian/stretch:latest AS LINKER

ARG LIBCAMERA_VERSION

WORKDIR /build/

RUN echo 'deb http://raspbian.raspberrypi.org/raspbian/ bullseye main contrib non-free rpi' > /etc/apt/sources.list \
  && echo 'deb http://archive.raspberrypi.org/debian/ bullseye main' >> /etc/apt/sources.list \
  && apt-get update \
  && apt-get install -y \
    build-essential \
    libcamera-dev \
    libevent-dev \
    libgc-dev \
    libgmp-dev \
    libpcre2-dev \
    libturbojpeg0-dev \
    zlib1g-dev \
  && rm -rf /var/lib/apt/lists/* \
  && mkdir /tmp/libcamera \
  && wget -P /tmp/libcamera  http://archive.raspberrypi.org/debian/pool/main/libc/libcamera/libcamera-dev_${LIBCAMERA_VERSION}_armhf.deb \
  && wget -P /tmp/libcamera  http://archive.raspberrypi.org/debian/pool/main/libc/libcamera/libcamera0_${LIBCAMERA_VERSION}_armhf.deb \
  && dpkg -iR /tmp/libcamera \
  && rm -rf /tmp/libcamera

COPY --from=BUILDER /build/bin/pi-camera.o bin/
COPY --from=BUILDER /build/link.sh .
COPY --from=LIBCAMERA_C_API /build/libcamera_c_api.a lib/
COPY --from=LIBBCM2835 /build/src/libbcm2835.a lib/

RUN . ./link.sh \
  && strip bin/pi-camera

ENTRYPOINT ["/bin/bash"]

FROM busybox:latest AS DEPLOYER

WORKDIR /export/

COPY --from=LINKER /build/bin/pi-camera .

CMD cp /export/pi-camera bin/ \
  && chown `stat -c %u:%g bin` bin/pi-camera
