#ifndef CREATE_H
#define CREATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "score.h"
#include "search.h"

struct Adjacency {
  short int ** matrix;
  size_t dim;
} * A1, * A2;

struct Graph {
  short int * taken;
  short int * translate;
  unsigned int ** sequence; // [tail_id][head_id] = score
  short int ** sequence_map;
  short int ** sequence_adj;
  short int ** outgoing; // [tail_id][0..num_outgoing] = head_id
  short int ** incoming;
  unsigned int * num_sequences;
  unsigned int * num_outgoing;
  unsigned int * num_incoming;
  unsigned int num_edges;
  unsigned int num_nodes;
  char ** id2name;
  short int * name2id;
} * G1, * G2;

struct Alignment {
  unsigned int topology_score;
  unsigned int sequence_score;
  double score;
  double alpha;
  short int last_move[4];
} * A;

void create_adj(struct Adjacency *, struct Graph *);
void create_graph(char *, struct Graph *);
void create_sequence(char *);
void create_alignment(char * [], double);

#endif
