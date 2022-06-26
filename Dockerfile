ARG image=arm32v5/debian:bullseye
FROM ${image}

RUN apt-get update && \
  apt-get install -y \
    build-essential \
    cmake \
    git \
    golang-go \
    libboost-program-options-dev \
    libbsd-dev \
    libexif-dev \
    libgnutls28-dev \
    libjpeg-dev \
    libpng-dev \
    libtiff-dev \
    meson \
    pkg-config \
    python3-jinja2 \
    python3-ply \
    python3-setuptools \
    python3-yaml \
    wget \
    && \
  rm -rf /var/lib/apt/lists/*

ENV LD_LIBRARY_PATH=libcamera-apps/build/core:libcamera-apps/build/image:libcamera-apps/build/post_processing_stages:libcamera-apps/build/preview:libcamera/build/src/libcamera/base:libcamera/build/src/libcamera

CMD ["make", "rebuild"]
