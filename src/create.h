#ifndef CREATE_H
#define CREATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "topology.h"

void create_adj(struct Adjacency *, struct Graph *);
int create_graph(char *, struct Graph *);
void create_alignment(char * files[]);

#endif
