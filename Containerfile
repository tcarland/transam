FROM ghcr.io/tcarland/tcalibcore:latest

RUN apt-get update && apt-get install -y --no-install-recommends \
    ffmpeg \
    flac \
    lame \
    libtag1-dev \
    libvorbis-dev \
    sox \
    vorbis-tools

WORKDIR / 

COPY . /transam
RUN cd transam && make

ENTRYPOINT ["/transam/transam"]