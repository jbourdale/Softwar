FROM debian:9

WORKDIR /app

RUN apt-get update
RUN apt-get install -y libzmq3-dev libczmq-dev libjansson-dev make gcc

ADD . /app
RUN make

ENTRYPOINT ["./softwar_server"]