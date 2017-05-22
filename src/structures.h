#ifndef STRUCTURES_H
#define STRUCTURES_H

struct Node {
  char * name;
  int id;
};

struct Edge {
  struct Node * tail;
  struct Node * head;
};

struct Adjacency {
  int ** matrix;
  size_t dim;
} * A1, * A2;

struct Graph {
  struct Node ** nodes;
  struct Edge ** edges;
  int num_nodes;
  int num_edges;
  Trie * n2e;
} * G1, * G2;

struct Mapping {
  Trie * translation;
  size_t num_mappings;
};

struct Alignment {
  struct Mapping * map;
  double score;
};

#endif
