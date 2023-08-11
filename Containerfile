FROM ghcr.io/tcarland/tcalibcore:latest

RUN apt-get install -y --no-install-recommends \
    ffmpeg \
    flac \
    lame \
    libtag1-dev \
    libvorbis-dev \
    sox \
    vorbis-tools

WORKDIR /opt 

RUN mkdir -p /opt/transam
COPY . /opt/transam

RUN cd transam && make