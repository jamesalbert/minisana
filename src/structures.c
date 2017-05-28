#include "structures.h"

short int name2id(char * name, struct Graph * graph) {
  int hashed = hash(name);
  while (strcmp(
    name, graph->id2name[graph->name2id[hashed++ % graph->num_nodes]]) != 0);
  return graph->name2id[--hashed % graph->num_nodes];
}

void associate_name(char * name, short int id, struct Graph * graph) {
  int hashed = hash(name);
  while (graph->name2id[
    hashed++ % graph->num_nodes] != (unsigned int)-1);
  graph->name2id[--hashed % graph->num_nodes] = id;
}
