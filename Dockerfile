FROM ubuntu:16.04

RUN apt-get update
RUN apt-get install -y gcc make

ADD . ./minisana
WORKDIR ./minisana
RUN make
RUN echo ${PWD}

CMD ["sh", "-c", "./bin/mini ${MINIOPTS}"]
