#ifndef OPTS_H
#define OPTS_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "structures.h"

int parse_args(int, char * [], struct MiniMan *);

#endif
