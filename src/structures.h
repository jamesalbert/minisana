#ifndef STRUCTURES_H
#define STRUCTURES_H

struct Adjacency {
  short int ** matrix;
  size_t dim;
} * A1, * A2;

struct Graph {
  short int * taken;
  short int * translate;
  short int ** edges;
  short int * num_outgoing;
  unsigned int num_nodes;
  unsigned int num_edges;
  char ** id2name;
} * G1, * G2;

struct Alignment {
  unsigned int score;
  int last_move[4];
} * A;

#endif
