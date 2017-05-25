#include "topology.h"

int edge_coverage(int node) { // u = name
  int score = 0;
  unsigned int translated_node, translated_head, translated_tail, head, tail;
  translated_node = G1->translate[node];
  for (size_t i = 0; i < G1->num_outgoing[node]; i++) {
    head = G1->outgoing_edges[node][i];
    translated_head = G1->translate[head]; // a(v) = translated_head->name
    if (A2->matrix[translated_node][translated_head] == 1)
      ++score;
  }
  for (size_t i = 0; i < G1->num_incoming[node]; i++) {
    tail = G1->incoming_edges[node][i];
    translated_tail = G1->translate[tail]; // a(v) = translated_head->name
    if (A2->matrix[translated_tail][translated_node] == 1)
      ++score;
  }
  return score;
}

int full_edge_coverage() {
  int score = 0;
  for (int i = 0; i < G1->num_nodes; i++)
    score += edge_coverage(i);
  return score / 2;
}
