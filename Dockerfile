FROM ubuntu:16.04

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt-get -y install software-properties-common
RUN add-apt-repository ppa:snaipewastaken/ppa
RUN apt-get update
RUN apt-get install -y gcc make criterion-dev

ADD . ./minisana
WORKDIR ./minisana
RUN make test

CMD ["sh", "-c", "./bin/mini ${MINIOPTS}"]
