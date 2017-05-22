#ifndef TOPOLOGY_H
#define TOPOLOGY_H

double full_edge_coverage(struct Alignment * alignment) {
  double Ea = 0.0;
  struct Node * translated_tail, * translated_head;
  for (size_t i = 0; i < alignment->map->smaller->num_edges; i++) {
    translated_tail = translate(alignment, alignment->map->smaller->edges[i]->tail->name);
    translated_head = translate(alignment, alignment->map->smaller->edges[i]->head->name);
    if (translated_tail != NULL && translated_head != NULL && alignment->a2->matrix[translated_tail->id][translated_head->id] == 1)
      Ea++;
  }
  return Ea / (double)alignment->map->smaller->num_edges;
}

double edge_coverage(struct Alignment * alignment, char * name) {
  double score = 0.0;
  struct Node * translated_node, * translated_head, * head;
  ArrayList * edges;
  translated_node = translate(alignment, name);
  edges = (ArrayList *)trie_lookup(alignment->map->smaller->n2e, name);
  for (size_t i = 0; i < edges->length; i++) {
    head = (struct Node *)edges->data[i];
    translated_head = translate(alignment, head->name);
    if (translated_node != NULL && translated_head != NULL && alignment->a2->matrix[translated_node->id][translated_head->id] == 1)
      score++;
  }
  return score / (double)alignment->map->smaller->num_edges;
}

double update_edge_coverage(struct Alignment * alignment, char * name1, char * name2) {
  double Ea = 0.0;
  Ea += edge_coverage(alignment, name1);
  if (name2 != NULL)
    Ea += edge_coverage(alignment, name2);
  return alignment->score + Ea;
}

#endif
