FROM debian
RUN apt-get update && apt-get install -y curl build-essential valgrind
ADD . /app
WORKDIR /app
RUN make re
