#include "destroy.h"

void free_adj(struct Adjacency * adj) {
  for (size_t i = 0; i < adj->dim; i++)
    free(adj->matrix[i]);
  free(adj->matrix);
  free(adj);
}

void free_graph(struct Graph * G) {
  for (size_t i = 0; i < G->num_nodes; i++) {
    free(G->outgoing[i]);
    free(G->id2name[i]);
  }
  free(G->outgoing);
  free(G->id2name);
}

void free_everything() {
  free(A);
  free(G1->num_outgoing);
  free(G2->num_outgoing);
  free(G1->translate);
  free(G2->taken);
  free_graph(G1);
  free_graph(G2);
  free_adj(A1);
  // free_adj(A2);
}
