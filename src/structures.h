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
};

struct Graph {
  struct Node ** nodes;
  struct Edge ** edges;
  int num_nodes;
  int num_edges;
} * G1, * G2;

struct Mapping {
  Trie * translation;
  // int * translation;
  struct Graph * smaller;
  struct Graph * larger;
  size_t num_mappings;
};

struct Alignment {
  struct Mapping * map;
  struct Adjacency * a1;
  struct Adjacency * a2;
  double score;
};

#endif
