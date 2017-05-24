CC = gcc

#CXXFLAGS = -Wall -fno-inline -O2 -std=c++11 -g
#CXXFLAGS = -std=c11 -O3 -fno-inline
CXXFLAGS = -std=c11
# CXXFLAGS = -U__STRICT_ANSI__ -Wall -std=c++11 -O3 #-ggdb -static -Bstatic

LIBS = -lm

OPTS = -ggdb

SRC =   src/mini.c \
  src/create.c \
  src/destroy.c \
  src/print.c \
  src/topology.c \
  src/sequence.c \

OBJDIR = _objs
OBJS = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))
MAIN = bin/mini

all: $(MAIN)

$(MAIN): $(OBJS)
	@mkdir bin
	$(CC) $(CXXFLAGS) -o $(MAIN) $(OBJS) $(LIBS)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CXXFLAGS)

clean:
	find . -type f | xargs touch
	rm -rf bin _objs
