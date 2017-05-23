#ifndef TOPOLOGY_H
#define TOPOLOGY_H

double edge_coverage(struct Alignment * alignment, char * name) { // u = name
  double score = 0.0;
  struct Node * translated_node, * translated_head, * head;
  ArrayList * edges;
  translated_node = translate(alignment, name); // a(u) = translated_node->name
  edges = (ArrayList *)trie_lookup(G1->n2e, name); // u's edges (potential v's)
  for (size_t i = 0; i < edges->length; i++) {
    head = (struct Node *)edges->data[i]; // assume v = head->name
    translated_head = translate(alignment, head->name); // a(v) = translated_head->name
    if (translated_node != NULL && translated_head != NULL &&
        A2->matrix[translated_node->id][translated_head->id] == 1)
      score++;
  }
  return score / (double)G1->num_edges;
}

double full_edge_coverage(struct Alignment * alignment) {
  double score = 0.0;
  for (size_t i = 0; i < G1->num_nodes; i++) {
    score += edge_coverage(alignment, G1->nodes[i]->name);
  }
  return score;
}

double small_edge_coverage(struct Alignment * alignment, char * name1, char * name2) {
  double score = edge_coverage(alignment, name1);
  if (name2 != NULL)
    score += edge_coverage(alignment, name2);
  return score;
}

#endif
