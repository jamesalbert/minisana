#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include "search.h"

typedef struct {
  short int ** matrix;
  size_t dim;
} Adjacency_t;

typedef struct {
  short int * taken;
  short int * translate;
  double ** sequence; // [tail_id][head_id] = score
  short int ** sequence_map; // [tail_id][i] = head_id
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
} Graph_t;

typedef struct {
  unsigned int topology_score;
  double sequence_score;
  double score;
  short int last_move[4];
} Alignment_t;

struct ConfigFile {
  FILE * handle;
  size_t len;
  char * line;
} * File;

typedef struct {
  Alignment_t * A;
  Adjacency_t * A1;
  Adjacency_t * A2;
  Graph_t * G1;
  Graph_t * G2;
  char * g1_file;
  char * g2_file;
  char * seq_file;
  char * output_file;
  double alpha;
  int time;
} MiniMan_t;

short int name2id(char *, Graph_t *);
void associate_name(char *, short int, Graph_t *);

#endif
