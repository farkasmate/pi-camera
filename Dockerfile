# syntax=docker/dockerfile:1.17
FROM crystallang/crystal:1.17.1-alpine-build AS builder

ENV CRYSTAL_CACHE_DIR=/root/.cache/crystal
ENV CRYSTAL_LIBRARY_PATH=/build/lib

WORKDIR /build/

COPY shard.yml shard.lock .

RUN --mount=type=cache,target=/root/.cache/crystal \
  shards install

COPY spec/ spec/
COPY src/ src/

# TODO: Add `--release`
RUN --mount=type=cache,target=/root/.cache/crystal \
  set -o pipefail \
  && shards --production build \
  --cross-compile \
  --target=aarch64-linux-gnu \
  | tee /tmp/build.log \
  && grep '^cc' /tmp/build.log > link.sh

FROM --platform=linux/arm64 debian:bookworm AS libbcm2835

WORKDIR /build/

RUN apt-get update \
  && apt-get install -y \
    build-essential \
    wget \
  && rm -rf /var/lib/apt/lists/*

RUN wget --quiet http://www.airspayce.com/mikem/bcm2835/bcm2835-1.75.tar.gz -O - | tar xvz \
  && ./bcm2835-1.75/configure \
  && make

FROM --platform=linux/arm64 debian:bookworm AS libcamera_c_api

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
    libcamera0.5 \
  && rm -rf /var/lib/apt/lists/*

COPY libcamera-c_api/*.cpp .
COPY libcamera-c_api/*.h .
COPY libcamera-c_api/Makefile .

RUN make

FROM --platform=linux/arm64 debian:bookworm AS linker

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
    libcamera0.5 \
    libevent-dev \
    libgc-dev \
    libgmp-dev \
    libpcre2-dev \
    libssl-dev \
    libturbojpeg0-dev \
    libyaml-dev \
    zlib1g-dev \
  && rm -rf /var/lib/apt/lists/*

COPY --from=builder /build/bin/epd.o bin/
COPY --from=builder /build/bin/pi-camera.o bin/
COPY --from=builder /build/link.sh .
COPY --from=libcamera_c_api /build/libcamera_c_api.a lib/
COPY --from=libbcm2835 /build/src/libbcm2835.a lib/

RUN bash -eu link.sh \
  && strip bin/epd \
  && strip bin/pi-camera

ENTRYPOINT ["/bin/bash"]

FROM busybox:latest AS deployer

WORKDIR /export/

COPY --from=LINKER /build/bin/epd .
COPY --from=LINKER /build/bin/pi-camera .

CMD cp /export/* bin/ \
  && chown `stat -c %u:%g bin` bin/*
