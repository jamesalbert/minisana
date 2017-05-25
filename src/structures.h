#ifndef STRUCTURES_H
#define STRUCTURES_H

struct Adjacency {
  short int ** matrix;
  size_t dim;
} * A1, * A2;

struct Graph {
  short int * taken;
  short int * translate;
  short int ** outgoing_edges;
  short int ** incoming_edges;
  unsigned int * num_outgoing;
  unsigned int * num_incoming;
  unsigned int num_edges;
  short int num_nodes;
  char ** id2name;
} * G1, * G2;

struct Alignment {
  int score;
  short int last_move[4];
} * A;

#endif
