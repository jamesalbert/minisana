#ifndef STRUCTURES_H
#define STRUCTURES_H

struct Adjacency {
  int ** matrix;
  size_t dim;
} * A1, * A2;

struct Graph {
  short int * taken;
  short int * translate;
  short int ** edges;
  short int * num_outgoing;
  short int num_nodes;
  short int num_edges;
  char ** id2name;
} * G1, * G2;

struct Alignment {
  double score;
  int last_move[4];
} * A;

#endif
