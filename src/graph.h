#ifndef GRAPH_H
#define GRAPH_H

struct Node * translate(struct Alignment * alignment, char * node) {
  return (struct Node *)trie_lookup(alignment->map->translation, node);
}

void insert_edge(struct Graph * graph, int index, int tail, int head) {
  graph->edges[index] = malloc(sizeof(struct Edge));
  graph->edges[index]->tail = graph->nodes[tail-1];
  graph->edges[index]->head = graph->nodes[head-1];
}

void insert_node(struct Graph * graph, int index, char * name) {
  graph->nodes[index] = malloc(sizeof(struct Node));
  graph->nodes[index]->name = malloc(strlen(name) * sizeof(char));
  graph->nodes[index]->id = index + 1;
  strcpy(graph->nodes[index]->name, name);
}

#endif
