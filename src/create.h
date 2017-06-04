#ifndef CREATE_H
#define CREATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "opts.h"
#include "read.h"
#include "score.h"
#include "search.h"
#include "structures.h"

Graph_t * new_graph();
Alignment_t * new_alignment();
MiniMan_t * new_miniman();
short ** create_adj(Graph_t *);
void ignore_header(Graph_t *);
void read_num_nodes(Graph_t *);
void read_nodes(Graph_t *);
void read_num_edges(Graph_t *);
void read_edges(Graph_t *);
void init_lists(MiniMan_t *);
void validate_file(MiniMan_t *);
void read_sequences(MiniMan_t *);
void create_graph(char *, Graph_t *);
void create_sequence(MiniMan_t *);
void create_alignment(MiniMan_t *);

#endif
