#ifndef GRAPH_H
#define GRAPH_H

/*

Changing a node:

let mapping_i = "g1[i] maps to g2[j]"
change(g1[i])
mapping_i => "g1[i] maps to g2[k]"


Swapping nodes:


let mapping_i = "g1[i] maps to g2[j]"
let mapping_x = "g1[x] maps to g2[y]"

swap(g1[i], g1[x]);

let mapping_i = "g1[i] maps to g2[y]"
let mapping_x = "g1[x] maps to g2[j]"

*/

struct Node * translate(struct Alignment * alignment, struct Node * node) {
  for (size_t i = 0; i < alignment->map->num_mappings; i++)
    if (alignment->map->tails[i] == node)
      return alignment->map->heads[i];
  return NULL;
}

void insert_edge(struct Graph * graph, int tail, int head) {
  graph->edges[graph->num_edges] = malloc(sizeof(struct Edge));
  graph->edges[graph->num_edges]->tail = graph->nodes[tail-1];
  graph->edges[graph->num_edges++]->head = graph->nodes[head-1];
}

void insert_node(struct Graph * graph, char * name) {
  graph->nodes[graph->num_nodes] = malloc(sizeof(struct Node));
  graph->nodes[graph->num_nodes]->name = malloc(strlen(name) * sizeof(char));
  graph->nodes[graph->num_nodes]->id = graph->num_nodes;
  strcpy(graph->nodes[graph->num_nodes++]->name, name);
}

#endif
