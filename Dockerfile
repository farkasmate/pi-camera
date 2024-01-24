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
  --target=aarch64-linux-gnu \
  | tee /tmp/build.log \
  && grep '^cc' /tmp/build.log > link.sh

FROM debian:bookworm AS LIBBCM2835

WORKDIR /build/

RUN apt-get update \
  && apt-get install -y \
    build-essential \
    wget \
  && rm -rf /var/lib/apt/lists/*

RUN wget --quiet http://www.airspayce.com/mikem/bcm2835/bcm2835-1.73.tar.gz -O - | tar xvz \
  && ./bcm2835-1.73/configure \
  && make

FROM debian:bookworm AS LIBCAMERA_C_API

WORKDIR /build/

RUN apt-get update \
  && apt-get install -y \
    gnupg

RUN echo 'deb http://archive.raspberrypi.com/debian/ bookworm main' > /etc/apt/sources.list \
  && apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 82B129927FA3303E \
  && apt-get update \
  && apt-get install -y \
    build-essential \
    libcamera-dev \
    libcamera0.1 \
  && rm -rf /var/lib/apt/lists/*

COPY libcamera-c_api/*.cpp .
COPY libcamera-c_api/*.h .
COPY libcamera-c_api/Makefile .

RUN make

FROM debian:bookworm AS LINKER

WORKDIR /build/

RUN apt-get update \
  && apt-get install -y \
    gnupg

RUN echo 'deb http://archive.raspberrypi.com/debian/ bookworm main' > /etc/apt/sources.list \
  && apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 82B129927FA3303E \
  && apt-get update \
  && apt-get install -y \
    build-essential \
    libcamera-dev \
    libcamera0.1 \
    libevent-dev \
    libgc-dev \
    libgmp-dev \
    libpcre2-dev \
    libssl-dev \
    libturbojpeg0-dev \
    libyaml-dev \
    zlib1g-dev \
  && rm -rf /var/lib/apt/lists/*

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
