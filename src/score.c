#include "score.h"

void edge_coverage(short int id, bool subtract) {
  unsigned int translated_node,
               translated_head,
               translated_tail,
               head, tail;
  translated_node = G1->translate[id];
  for (size_t i = 0; i < G1->num_outgoing[id]; i++) {
    head = G1->outgoing[id][i];
    translated_head = G1->translate[head];
    if (A2->matrix[translated_node][translated_head] == 1 ||
        A2->matrix[translated_head][translated_node] == 1)
      A->topology_score += (subtract ? -1 : 1);
  }
  for (size_t i = 0; i < G1->num_incoming[id]; i++) {
    tail = G1->incoming[id][i];
    translated_tail = G1->translate[tail];
    if (A2->matrix[translated_tail][translated_node] == 1 ||
        A2->matrix[translated_node][translated_tail] == 1)
      A->topology_score += (subtract ? -1 : 1);
  }
}

void full_edge_coverage() {
  unsigned int translated_node,
               translated_head,
               head;
  A->topology_score = 0;
  for (size_t i = 0; i < G1->num_nodes; i++) {
    translated_node = G1->translate[i];
    for (size_t j = 0; j < G1->num_outgoing[i]; j++) {
      head = G1->outgoing[i][j];
      translated_head = G1->translate[head];
      if (A2->matrix[translated_node][translated_head] == 1)
        ++A->topology_score;
    }
  }
}

void sequence_similarity(short int id, bool subtract) {
  short int translated_node = G1->translate[id];
  if (G1->sequence_adj[id][translated_node] == 0)
    return;
  A->sequence_score += (subtract ? -1 : 1) * G1->sequence[id][translated_node];
}

void full_sequence_similarity() {
  A->sequence_score = 0;
  for (short int i = 0; i < G1->num_nodes; i++)
    sequence_similarity(i, false);
}

void update_score() {
  double alpha = 0.999;
  double topo = (1.0-alpha)*(double)A->topology_score/G1->num_edges;
  double seq = alpha*(double)A->sequence_score/10000;
  A->score = topo/2 + seq/2;
}

void subtract_score(short int id) {
  edge_coverage(id, true);
  sequence_similarity(id, true);
}

void add_score(short int id) {
  edge_coverage(id, false);
  sequence_similarity(id, false);
}
