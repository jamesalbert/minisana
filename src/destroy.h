#ifndef DESTROY_H
#define DESTROY_H

#include <stdlib.h>
#include "structures.h"

void free_adj(struct Adjacency *);
void free_graph(struct Graph *);
void free_everything(struct Alignment *);

#endif
