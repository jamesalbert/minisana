#ifndef STRUCTURES_H
#define STRUCTURES_H

struct Adjacency {
  int ** matrix;
  size_t dim;
} * A1, * A2;

struct Graph {
  int * taken;
  int * translate;
  int ** edges;
  int * num_outgoing;
  int num_nodes;
  int num_edges;
  char ** id2name;
} * G1, * G2;

struct Alignment {
  int score;
  int last_move[4];
} * A;

#endif
