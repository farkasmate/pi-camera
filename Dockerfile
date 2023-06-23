FROM crystallang/crystal:1.8.2-alpine AS BUILDER

ENV CRYSTAL_LIBRARY_PATH=/build/lib

WORKDIR /build/

COPY shard.yml shard.lock .
COPY spec/ spec/
COPY src/ src/

RUN shards build \
  --cross-compile \
  --target=arm-linux-gnueabihf \
  | grep '^cc' > link.sh

FROM raspbian/stretch:latest AS LIBS

WORKDIR /build/

RUN echo 'deb http://raspbian.raspberrypi.org/raspbian/ bullseye main contrib non-free rpi' > /etc/apt/sources.list \
  && echo 'deb http://archive.raspberrypi.org/debian/ bullseye main' >> /etc/apt/sources.list \
  && apt-get update \
  && apt-get install -y \
    build-essential \
    wget \
  && rm -rf /var/lib/apt/lists/*

COPY epd/Makefile .
COPY epd/e-Paper/ e-Paper/

RUN make clean all

FROM raspbian/stretch:latest AS LINKER

WORKDIR /build/

RUN echo 'deb http://raspbian.raspberrypi.org/raspbian/ bullseye main contrib non-free rpi' > /etc/apt/sources.list \
  && echo 'deb http://archive.raspberrypi.org/debian/ bullseye main' >> /etc/apt/sources.list \
  && apt-get update \
  && apt-get install -y \
    build-essential \
    libevent-dev \
    libgc-dev \
    libgmp-dev \
    libpcre2-dev \
    zlib1g-dev \
  && rm -rf /var/lib/apt/lists/*

COPY --from=BUILDER /build/bin/pi-camera.o bin/
COPY --from=BUILDER /build/link.sh .
COPY --from=LIBS /build/libbcm2835.a /build/libepd_2in13_v2.a lib/

RUN . ./link.sh \
  && strip bin/pi-camera

ENTRYPOINT ["/bin/bash"]

FROM busybox:latest AS DEPLOYER

WORKDIR /export/

COPY --from=LINKER /build/bin/pi-camera .

CMD cp /export/pi-camera bin/ \
  && chown `stat -c %u:%g bin` bin/pi-camera
