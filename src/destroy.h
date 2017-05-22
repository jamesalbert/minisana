#ifndef DESTROY_H
#define DESTROY_H

void destroy_nodes(struct Node ** nodes, size_t num) {
  for (size_t i = 0; i < num; i++)
    free(nodes[i]);
  // printf("info: nodes freed\n");
}

void destroy_edges(struct Edge ** edges, size_t num) {
  for (size_t i = 0; i < num - 1; i++)
    free(edges[i]);
  // printf("info: edges freed\n");
}

void destroy_graph(struct Graph * graph) {
  destroy_nodes(graph->nodes, graph->num_nodes);
  destroy_edges(graph->edges, graph->num_edges);
  free(graph->nodes);
  free(graph->edges);
  free(graph);
  // printf("info: graph freed\n");
}

void destroy_mapping(struct Mapping * map) {
  trie_free(map->translation);
  free(map);
  // printf("info: map freed\n");
}

void destroy_adj(struct Adjacency * adj) {
  for (size_t i = 0; i < adj->dim; i++)
    free(adj->matrix[i]);
  free(adj->matrix);
  free(adj);
  // printf("info: adjacency freed\n");
}

void destroy_alignment(struct Alignment * a) {
  destroy_graph(a->map->smaller);
  destroy_graph(a->map->larger);
  destroy_mapping(a->map);
  // destroy_adj(a->a1);
  // destroy_adj(a->a2);
  free(a);
  // printf("info: alignment freed\n");
}

void destroy_alignment_copy(struct Alignment * a) {
  destroy_mapping(a->map);
  free(a);
  // printf("info: alignment copy freed\n");
}

#endif
