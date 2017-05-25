CC = gcc

#CXXFLAGS = -Wall -fno-inline -O2 -std=c++11 -g
#CXXFLAGS = -std=c11 -O3 -fno-inline
CXXFLAGS = -std=c11
# CXXFLAGS = -U__STRICT_ANSI__ -Wall -std=c++11 -O3 #-ggdb -static -Bstatic

LIBS = -lm -I/usr/local/include/libcalg-1.0/libcalg

OPTS = -ggdb

SRC = src/mini.c

MAIN = bin/mini

bin/mini: src/create.h src/destroy.h src/mini.c src/print.h src/structures.h src/topology.h
	$(CC) $(CXXFLAGS) $(SRC) $(OPTS) $(LIBS) -o $(MAIN)

clean:
	rm -f bin/mini src/*.o
