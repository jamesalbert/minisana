FROM ubuntu:16.04

RUN apt-get update
RUN apt-get install -y gcc make

ADD . ./minisana
RUN cd minisana && make

CMD ["./minisana/bin/mini", \
     "-n", "./minisana/input/yeast.gw", \
     "-N", "./minisana/input/human.gw", \
     "-s", "./minisana/input/yeast_human.bitscores"]
