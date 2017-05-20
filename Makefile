CC = gcc

#CXXFLAGS = -Wall -fno-inline -O2 -std=c++11 -g
CXXFLAGS = -std=c11
# CXXFLAGS = -U__STRICT_ANSI__ -Wall -std=c++11 -O3 #-ggdb -static -Bstatic

LIBS = -lm -lncurses

OPTS = -g

SRC = src/mini.c

MAIN = bin/mini

all:
	rm -rf bin
	mkdir bin
	$(CC) $(CXXFLAGS) $(SRC) $(OPTS) $(LIBS) -o $(MAIN)

clean:
	find . -type f | xargs touch
	rm -rf bin
