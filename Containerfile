FROM ghcr.io/tcarland/tcanetpp:latest


WORKDIR /opt 

RUN mkdir -p /opt/transam
COPY . /opt/transam

RUN cd transam && make