FROM ubuntu:20.04

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get -qq update && apt-get -qq dist-upgrade && apt-get install -qq -y --no-install-recommends \
    git \
    make \
    openssh-client \
    ca-certificates \
    #curl \
    #p7zip \
    build-essential \
    pkg-config \
    libgl1-mesa-dev \
    libsm6 \
    libice6 \
    libxext6 \
    libxrender1 \
    libfontconfig1 \
    qtbase5-dev  \
    && apt-get -qq clean

#Install QT

WORKDIR /eda
RUN git clone https://github.com/wulffern/ciccreator.git
WORKDIR ciccreator
RUN git pull
RUN make all
