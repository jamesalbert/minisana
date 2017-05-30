#ifndef PRINT_H
#define PRINT_H

#include <stdio.h>
#include <stdbool.h>
#include "structures.h"

void print_mapping(struct MiniMan *);
void print_status(struct Alignment *, double, int);
void print_node(short int, bool);
void print_sequences();

#endif
