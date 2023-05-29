FROM ubuntu:22.04

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get -qq update && apt-get install -qq -y --no-install-recommends \
    git \
    make \
    openssh-client \
    ca-certificates \

    build-essential \
    pkg-config \
    libgl1-mesa-dev \
    libsm6 \
    libice6 \
    libxext6 \
    libxrender1 \
    libfontconfig1 \
    qt6-base-dev  \
    python3 python3-pip jq\
    && apt-get -qq clean

RUN python3 -m pip install matplotlib numpy click svgwrite pyyaml pandas tabulate wheel setuptools tikzplotlib

ENTRYPOINT /bin/bash
