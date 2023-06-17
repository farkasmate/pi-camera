FROM raspbian/stretch:latest

RUN echo 'deb http://raspbian.raspberrypi.org/raspbian/ bullseye main contrib non-free rpi' > /etc/apt/sources.list \
  && echo 'deb http://archive.raspberrypi.org/debian/ bullseye main' >> /etc/apt/sources.list \
  && apt-get update \
  && apt-get install -y \
    build-essential \
    git \
    golang-go \
    libboost-program-options-dev \
    libbsd-dev \
    libcamera-dev \
    libexif-dev \
    libgnutls28-dev \
    libjpeg-dev \
    libturbojpeg0-dev \
    meson \
    pkg-config \
    python3-jinja2 \
    python3-ply \
    python3-setuptools \
    python3-yaml \
    wget \
  && rm -rf /var/lib/apt/lists/*

CMD ["make", "rebuild"]
