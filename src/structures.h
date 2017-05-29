#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include "search.h"

struct Adjacency {
  short int ** matrix;
  size_t dim;
} * A1, * A2;

struct Graph {
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
} * G1, * G2;

struct Alignment {
  unsigned int topology_score;
  double sequence_score;
  double score;
  short int last_move[4];
} * A;

struct ConfigFile {
  FILE * handle;
  size_t len;
  char * line;
  struct Graph * graph;
} * File;

struct MiniMan {
  char * g1_file;
  char * g2_file;
  char * seq_file;
  char * output_file;
  double alpha;
  int time;
} * mm;

short int name2id(char *, struct Graph *);
void associate_name(char *, short int, struct Graph *);

#endif
