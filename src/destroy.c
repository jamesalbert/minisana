#include "destroy.h"

void free_adjs(MiniMan_t * mm) {
  for (size_t i = 0; i < mm->G1->num_nodes; i++)
    free(mm->adjmat1[i]);
  for (size_t i = 0; i < mm->G2->num_nodes; i++)
    free(mm->adjmat2[i]);
  free(mm->adjmat1);
  free(mm->adjmat2);
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
  free_adjs(mm);
  free(mm);
}
