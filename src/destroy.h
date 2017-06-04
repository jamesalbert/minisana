#ifndef DESTROY_H
#define DESTROY_H

#include <stdlib.h>
#include "structures.h"

void free_adjs(MiniMan_t *);
void free_graph(Graph_t *);
void free_everything(MiniMan_t *);

#endif
