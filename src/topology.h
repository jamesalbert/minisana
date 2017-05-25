#ifndef TOPOLOGY_H
#define TOPOLOGY_H

int edge_coverage(int node) { // u = name
  if (G1->num_outgoing[node] == 0) {
    return 0;
  }
  int score = 0;
  int translated_node, translated_head, head;
  translated_node = G1->translate[node];
  for (size_t i = 0; i < G1->num_outgoing[node]; i++) {
    head = G1->edges[node][i];
    translated_head = G1->translate[head]; // a(v) = translated_head->name
    if (A2->matrix[translated_node][translated_head] == 1) {
      score += 1;
    }
  }
  return score;
}

int full_edge_coverage() {
  int score = 0;
  for (int i = 0; i < G1->num_nodes; i++)
    score += edge_coverage(i);
  return score;
}

#endif
