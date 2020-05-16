FROM debian
RUN mkdir -p /app
WORKDIR /app
RUN apt update && \ 
apt install -y curl build-essential git dh-autoreconf autotools-dev && \
git clone https://github.com/httperf/httperf && \
    cd httperf && \
    autoreconf -i && \
    ./configure && \
    make && \
    make install
CMD [ "httperf", "--server", "web", "--port", "1111", "--num-conns", "100", "--rate", "10" ]