CC = gcc

#CXXFLAGS = -Wall -fno-inline -O2 -std=c++11 -g
# CXXFLAGS = -U__STRICT_ANSI__ -Wall -std=c++11 -O3 #-ggdb -static -Bstatic

LIBS = -lm

SRC = src/c/mini.c

MAIN = bin/mini

all:
	mkdir bin
	$(CC) $(SRC) $(LIBS) -o $(MAIN)

clean:
	rm -rf bin
