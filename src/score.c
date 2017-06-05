#include "score.h"

void edge_coverage(MiniMan_t * mm, short int id, bool subtract) {
  unsigned int translated_node,
               translated_head,
               translated_tail,
               head, tail;
  translated_node = mm->G1->translate[id];
  unsigned int score = 0;
  unsigned int limit = fmax(mm->G1->num_outgoing[id], mm->G1->num_incoming[id]);
  size_t i;
  for (i = 0; i < limit; ++i) {
    if (i < mm->G1->num_outgoing[id]) {
      head = mm->G1->outgoing[id][i];
      translated_head = mm->G1->translate[head];
      if (mm->adjmat2[translated_node][translated_head] == 1 ||
          mm->adjmat2[translated_head][translated_node] == 1)
        score += (subtract ? -1 : 1);
    }
    if (i < mm->G1->num_incoming[id]) {
      tail = mm->G1->incoming[id][i];
      translated_tail = mm->G1->translate[tail];
      if (mm->adjmat2[translated_tail][translated_node] == 1 ||
          mm->adjmat2[translated_node][translated_tail] == 1)
        score += (subtract ? -1 : 1);
    }
  }
  mm->topology_score += score;
}

void full_edge_coverage(MiniMan_t * mm) {
  size_t i, j;
  unsigned int translated_node,
               translated_head,
               head;
  mm->topology_score = 0;
  for (i = 0; i < mm->G1->num_nodes; ++i) {
    translated_node = mm->G1->translate[i];
    for (j = 0; j < mm->G1->num_outgoing[i]; ++j) {
      head = mm->G1->outgoing[i][j];
      translated_head = mm->G1->translate[head];
      if (mm->adjmat2[translated_node][translated_head] == 1)
        ++mm->topology_score;
    }
  }
}

void sequence_similarity(MiniMan_t * mm, short int id, bool subtract) {
  short translated_node = mm->G1->translate[id];
  if (mm->G1->sequence_adj[id][translated_node] == 0)
    return;
  mm->sequence_score += (subtract ? -1 : 1) * mm->G1->sequence[id][translated_node];
}

void full_sequence_similarity(MiniMan_t * mm) {
  short i;
  mm->sequence_score = 0.0;
  for (i = 0; i < mm->G1->num_nodes; i++)
    sequence_similarity(mm, i, false);
}

void update_score(MiniMan_t * mm) {
  double topo = (1.0-mm->alpha)*(double)mm->topology_score/mm->G1->num_edges;
  double seq = mm->alpha*mm->sequence_score/mm->G2->num_nodes;
  mm->score = topo + seq;
}

void subtract_score(MiniMan_t * mm, short int id) {
  edge_coverage(mm, id, true);
  sequence_similarity(mm, id, true);
}

void add_score(MiniMan_t * mm, short int id) {
  edge_coverage(mm, id, false);
  sequence_similarity(mm, id, false);
}
