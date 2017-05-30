CC = gcc

#CXXFLAGS = -Wall -fno-inline -O2 -std=c++11 -g
#CXXFLAGS = -std=c11 -O3 -fno-inline -fno-stack-protector
CXXFLAGS = -std=c11 -O0
#CXXFLAGS = -std=c11
# CXXFLAGS = -U__STRICT_ANSI__ -Wall -std=c++11 -O3 #-ggdb -static -Bstatic

LIBS = -lm

OPTS = -ggdb

SRC =   src/mini.c \
  src/create.c \
  src/destroy.c \
  src/print.c \
  src/score.c \
  src/opts.c \
  src/search.c \
  src/read.c \
  src/structures.c \

OBJDIR = _objs
OBJS = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))
MAIN = bin/mini

all: $(MAIN)

$(MAIN): $(OBJS)
	-mkdir bin output lib
	$(CC) $(CXXFLAGS) $(OPTS) -o $(MAIN) $(OBJS) $(LIBS) -ftest-coverage -fprofile-arcs
	$(CC) -ggdb -shared -o lib/libmini.so $(OBJS) $(LIBS) -ftest-coverage -fprofile-arcs

$(OBJDIR)/%.o: %.c
	-mkdir -p $(dir $@)
	$(CC) -c -ftest-coverage -fprofile-arcs -fPIC -o $@ $< $(CXXFLAGS) $(OPTS)

test: $(MAIN)
	-mkdir coverage
	gcc -Llib -Isrc -o tests/run_tests tests/test_mini.c -lcriterion -lmini -lm
	LD_LIBRARY_PATH=lib ./tests/run_tests --verbose --xml=coverage/report.xml
	gcovr -r . -d -p --html --html-details -o coverage/report.html
	@echo \"open coverage/report.html\" to view coverage

docker:
	docker build -t minisana .
	-docker rmi -f $$(docker images --filter "dangling=true" -q)

docker_run: docker
	-mkdir output
	docker run -v $(PWD)/output:/minisana/output -e "MINIOPTS=$(MINIOPTS)" -dt minisana

docker_clean:
	-docker stop $$(docker ps -q --filter ancestor=minisana )
	-docker rmi -f minisana

clean: docker_clean
	@find . -type f | xargs touch
	-rm -rf bin output _objs lib tests/run_tests coverage
