#ifndef DESTROY_H
#define DESTROY_H

#include <stdlib.h>
#include "create.h"

void free_adj(struct Adjacency *);
void free_graph(struct Graph *);
void free_everything();

#endif
