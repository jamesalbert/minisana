#ifndef DESTROY_H
#define DESTROY_H

void destroy_nodes(struct Node ** nodes, size_t num) {
  for (size_t i = 0; i < num; i++) {
    free(nodes[i]->name);
    free(nodes[i]);
  }
}

void destroy_edges(struct Edge ** edges, size_t num) {
  for (size_t i = 0; i < num - 1; i++)
    free(edges[i]);
}

void destroy_graph(struct Graph * graph) {
  trie_free(graph->n2e);
  destroy_nodes(graph->nodes, graph->num_nodes);
  destroy_edges(graph->edges, graph->num_edges);
  free(graph->nodes);
  free(graph->edges);
  free(graph);
}

void destroy_adj(struct Adjacency * adj) {
  for (size_t i = 0; i < adj->dim; i++) {
    free(adj->matrix[i]);
  }
  free(adj->matrix);
  free(adj);
}

void destroy_alignment(struct Alignment * a) {
  destroy_graph(G1);
  destroy_graph(G2);
  trie_free(a->map);
  destroy_adj(A1);
  destroy_adj(A2);
  free(a);
}

void destroy_alignment_copy(struct Alignment * a) {
  trie_free(a->map);
  free(a);
}

#endif
