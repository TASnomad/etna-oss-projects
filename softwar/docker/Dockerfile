FROM debian:stretch

ARG PROJECT_DIR=.
ENV HOME=/home

WORKDIR $HOME/cocoestbelle

RUN apt-get update && \
  apt-get install --assume-yes pkgconf && \
  apt-get install --assume-yes libzmq3-dev && \
  apt-get install --assume-yes build-essential && \
  apt-get install --assume-yes libczmq-dev && \
  apt-get install --assume-yes libjson-c-dev

COPY $PROJECT_DIR $HOME/cocoestbelle

RUN make