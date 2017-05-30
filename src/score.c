#include "score.h"

void edge_coverage(MiniMan_t * mm, short int id, bool subtract) {
  unsigned int translated_node,
               translated_head,
               translated_tail,
               head, tail;
  translated_node = mm->G1->translate[id];
  for (size_t i = 0; i < mm->G1->num_outgoing[id]; ++i) {
    head = mm->G1->outgoing[id][i];
    translated_head = mm->G1->translate[head];
    if (mm->A2->matrix[translated_node][translated_head] == 1 ||
        mm->A2->matrix[translated_head][translated_node] == 1)
      mm->A->topology_score += (subtract ? -1 : 1);
  }
  for (size_t i = 0; i < mm->G1->num_incoming[id]; ++i) {
    tail = mm->G1->incoming[id][i];
    translated_tail = mm->G1->translate[tail];
    if (mm->A2->matrix[translated_tail][translated_node] == 1 ||
        mm->A2->matrix[translated_node][translated_tail] == 1)
      mm->A->topology_score += (subtract ? -1 : 1);
  }
}

void full_edge_coverage(MiniMan_t * mm) {
  unsigned int translated_node,
               translated_head,
               head;
  mm->A->topology_score = 0;
  for (size_t i = 0; i < mm->G1->num_nodes; ++i) {
    translated_node = mm->G1->translate[i];
    for (size_t j = 0; j < mm->G1->num_outgoing[i]; ++j) {
      head = mm->G1->outgoing[i][j];
      translated_head = mm->G1->translate[head];
      if (mm->A2->matrix[translated_node][translated_head] == 1)
        ++mm->A->topology_score;
    }
  }
}

void sequence_similarity(MiniMan_t * mm, short int id, bool subtract) {
  short int translated_node = mm->G1->translate[id];
  if (mm->G1->sequence_adj[id][translated_node] == 0)
    return;
  mm->A->sequence_score += (subtract ? -1 : 1) * mm->G1->sequence[id][translated_node];
}

void full_sequence_similarity(MiniMan_t * mm) {
  mm->A->sequence_score = 0.0;
  for (short int i = 0; i < mm->G1->num_nodes; i++)
    sequence_similarity(mm, i, false);
}

void update_score(MiniMan_t * mm) {
  double topo = (1.0-mm->alpha)*(double)mm->A->topology_score/mm->G1->num_edges;
  double seq = mm->alpha*mm->A->sequence_score/mm->G2->num_nodes;
  mm->A->score = topo + seq;
}

void subtract_score(MiniMan_t * mm, short int id) {
  edge_coverage(mm, id, true);
  sequence_similarity(mm, id, true);
}

void add_score(MiniMan_t * mm, short int id) {
  edge_coverage(mm, id, false);
  sequence_similarity(mm, id, false);
}
