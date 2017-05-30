#ifndef PRINT_H
#define PRINT_H

#include <stdio.h>
#include <stdbool.h>
#include "structures.h"

void print_mapping(MiniMan_t *);
void print_status(MiniMan_t *, double, int);
void print_node(MiniMan_t *, short int, bool);
void print_sequences(MiniMan_t * mm);

#endif
