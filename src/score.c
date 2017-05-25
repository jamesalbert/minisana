#include "score.h"

unsigned int edge_coverage(short int id) {
  unsigned int score = 0,
               translated_node,
               translated_head,
               translated_tail,
               head, tail;
  translated_node = G1->translate[id];
  for (size_t i = 0; i < G1->num_outgoing[id]; i++) {
    head = G1->outgoing[id][i];
    translated_head = G1->translate[head];
    if (A2->matrix[translated_node][translated_head] == 1)
      ++score;
  }
  for (size_t i = 0; i < G1->num_incoming[id]; i++) {
    tail = G1->incoming[id][i];
    translated_tail = G1->translate[tail];
    if (A2->matrix[translated_tail][translated_node] == 1)
      ++score;
  }
  return score;
}

unsigned int full_edge_coverage() {
  unsigned int score = 0,
               translated_node,
               translated_head,
               head;
  for (size_t i = 0; i < G1->num_nodes; i++) {
    translated_node = G1->translate[i];
    for (size_t j = 0; j < G1->num_outgoing[i]; j++) {
      head = G1->outgoing[i][j];
      translated_head = G1->translate[head];
      if (A2->matrix[translated_node][translated_head] == 1)
        ++score;
    }
  }
  return score;
}

unsigned int sequence_similarity(short int id) {
  short int translated_node = G1->translate[id];
  if (G1->sequence_adj[id][translated_node] == 0)
    return 0;
  return G1->sequence[id][translated_node];
}

unsigned int full_sequence_similarity() {
  int score = 0;
  for (short int i = 0; i < G1->num_nodes; i++) {
    score += sequence_similarity(i);
  }
  return score;
}

void update_score() {
  double topo = 0.99*(double)A->topology_score/G1->num_edges;
  double seq = 0.01*(double)A->sequence_score/10000;
  A->score = topo + seq;
}

void subtract_score(short int id) {
  A->topology_score -= edge_coverage(id);
  A->sequence_score -= sequence_similarity(id);
}

void add_score(short int id) {
  A->topology_score += edge_coverage(id);
  A->sequence_score += sequence_similarity(id);
}
