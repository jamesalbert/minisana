#ifndef CREATE_H
#define CREATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read.h"
#include "score.h"
#include "search.h"
#include "structures.h"

void create_adj(struct Adjacency *, struct Graph *);
void ignore_header();
void read_num_nodes();
void read_nodes();
void read_num_edges();
void read_edges();
void init_lists();
void validate_file();
void read_sequences();
void create_graph(char *, struct Graph *);
void create_sequence(char *);
void create_alignment(struct Alignment *, struct MiniMan *);

#endif
