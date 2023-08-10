FROM ghcr.io/tcarland/tcanetpp:latest

RUN apt-get install -y --no-install-recommends \
    libtag1-dev

WORKDIR /opt 

RUN mkdir -p /opt/transam
COPY . /opt/transam

RUN cd transam && make