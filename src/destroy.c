#include "destroy.h"

void free_adj(Adjacency_t * adj) {
  for (size_t i = 0; i < adj->dim; i++)
    free(adj->matrix[i]);
  free(adj->matrix);
  free(adj);
}

void free_graph(Graph_t * G) {
  for (size_t i = 0; i < G->num_nodes; i++) {
    free(G->outgoing[i]);
    free(G->id2name[i]);
  }
  free(G->outgoing);
  free(G->id2name);
}

void free_everything(MiniMan_t * mm) {
  free(mm->A);
  free(mm->G1->num_outgoing);
  free(mm->G2->num_outgoing);
  free(mm->G1->translate);
  free(mm->G2->taken);
  free_graph(mm->G1);
  free_graph(mm->G2);
  free_adj(mm->A1);
  free(mm);
  // free_adj(A2);
}
