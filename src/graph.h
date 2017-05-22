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

struct Node * translate(struct Alignment * alignment, char * node) {
  // for (size_t i = 0; i < alignment->map->num_mappings; i++)
  //   if (alignment->map->tails[i] == node)
  //     return alignment->map->heads[i];
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
