#ifndef TOPOLOGY_H
#define TOPOLOGY_H

double edge_coverage(struct Alignment * alignment) {
  double Ea = 0.0;
  struct Node * translated_tail, * translated_head;
  for (size_t i = 0; i < alignment->map->smaller->num_edges; i++) {
    translated_tail = translate(alignment, alignment->map->smaller->edges[i]->tail);
    translated_head = translate(alignment, alignment->map->smaller->edges[i]->head);
    if (translated_tail == NULL || translated_head == NULL)
      continue;
    if (alignment->a2->matrix[translated_tail->id][translated_head->id] == 1)
      Ea++;
  }
  printf("ec score: %d\n", alignment->map->smaller->num_edges);
  return Ea / (double)alignment->map->smaller->num_edges;
}

#endif
