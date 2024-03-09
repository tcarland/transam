FROM ghcr.io/tcarland/tcalibcore:latest

USER root

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
RUN cd transam && source .resources/build_profile.release && make

USER tdh

ENTRYPOINT ["/transam/transam"]