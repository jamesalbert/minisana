#ifndef DESTROY_H
#define DESTROY_H

void destroy_nodes(struct Node ** nodes, size_t num) {
  for (size_t i = 0; i < num; i++)
    free(nodes[i]);
}

void destroy_edges(struct Edge ** edges, size_t num) {
  for (size_t i = 0; i < num - 1; i++)
    free(edges[i]);
}

void destroy_graph(struct Graph * graph) {
  destroy_nodes(graph->nodes, graph->num_nodes);
  destroy_edges(graph->edges, graph->num_edges);
  free(graph->nodes);
  free(graph->edges);
  free(graph);
}

void destroy_mapping(struct Mapping * map) {
  free(map->tails);
  free(map->heads);
  free(map);
}

void destroy_adj(struct Adjacency * adj) {
  for (size_t i = 0; i < adj->dim; i++)
    free(adj->matrix[i]);
  free(adj->matrix);
  free(adj);
}

void destroy_alignment(struct Alignment * a) {
  destroy_graph(a->map->smaller);
  destroy_graph(a->map->larger);
  destroy_mapping(a->map);
  destroy_adj(a->a1);
  destroy_adj(a->a2);
  free(a);
}

void destroy_alignment_copy(struct Alignment * a) {
  destroy_mapping(a->map);
  free(a);
}

#endif
