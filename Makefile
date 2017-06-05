CC = gcc
CXXFLAGS = -std=c11 -O3 -fno-inline -fno-stack-protector -fPIC
OBJDIR = _objs
MAIN = bin/mini
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
  src/structures.c
OBJS = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))

all: $(MAIN)

$(MAIN): $(OBJS)
	-mkdir bin output lib
	$(CC) $(CXXFLAGS) $(OPTS) -o $(MAIN) $(OBJS) $(LIBS)
	$(CC) $(CXXFLAGS) $(OPTS) -shared -o lib/libmini.so $(OBJS) $(LIBS)

$(OBJDIR)/%.o: %.c
	-mkdir -p $(dir $@)
	$(CC) -c $(OBJFLAGS) -o $@ $< $(CXXFLAGS) $(OPTS)

test: CXXFLAGS=-std=c11 -O0 -ftest-coverage -fprofile-arcs
test: OBJFLAGS=-ftest-coverage -fprofile-arcs -fPIC
test: clean $(MAIN)
	-mkdir coverage
	$(CC) -Llib -Isrc -o tests/run_tests tests/test_mini.c -lcriterion -lmini -lm
	LD_LIBRARY_PATH=lib ./tests/run_tests --verbose
	gcovr -r . -p --html --html-details -o coverage/report.html
	gcovr -r . --xml-pretty -o coverage/pretty-report.xml
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
