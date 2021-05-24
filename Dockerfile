FROM ubuntu:bionic

RUN apt-get update -q && \
    DEBIAN_FRONTEND=noninteractive apt-get install -qy make git && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

#- Copied from  https://github.com/rabits/dockerfiles/blob/master/5.4-desktop/Dockerfile
#- Install QT
RUN apt-get -qq update && apt-get -qq dist-upgrade && apt-get install -qq -y --no-install-recommends \
    git \
    openssh-client \
    ca-certificates \
    curl \
    p7zip \
    build-essential \
    pkg-config \
    libgl1-mesa-dev \
    libsm6 \
    libice6 \
    libxext6 \
    libxrender1 \
    libfontconfig1 \
    && apt-get -qq clean

#Install QT
ENV DEBIAN_FRONTEND noninteractive
ENV QT_PATH /opt/Qt
ENV QT_DESKTOP $QT_PATH/5.4/gcc_64
ENV PATH $QT_DESKTOP/bin:$PATH

# Download & unpack Qt 5.4 toolchains & clean
RUN mkdir -p /tmp/qt \
    && curl -Lo /tmp/qt/installer.run 'http://simvascular.stanford.edu/downloads/public/open_source/linux/qt/5.4/qt-opensource-linux-x64-5.4.2.run'


RUN chmod 755 /tmp/qt/installer.run && /tmp/qt/installer.run --dump-binary-data -o /tmp/qt/data || exit 0;

RUN mkdir $QT_PATH && cd $QT_PATH \
    && 7zr x /tmp/qt/data/qt.54.gcc_64/5.4.2-0qt5_essentials.7z > /dev/null \
    && 7zr x /tmp/qt/data/qt.54.gcc_64/5.4.2-0qt5_addons.7z > /dev/null \
    && 7zr x /tmp/qt/data/qt.54.gcc_64/5.4.2-0icu_53_1_ubuntu_11_10_64.7z > /dev/null \
    && /tmp/qt/installer.run --runoperation QtPatch linux $QT_DESKTOP qt5 || exit 0;
    #&& rm -rf /tmp/qt

RUN git clone https://github.com/wulffern/ciccreator.git
WORKDIR ciccreator
#RUN git checkout develop
RUN git pull
RUN make all
