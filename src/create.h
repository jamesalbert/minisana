#ifndef CREATE_H
#define CREATE_H

void create_adj(struct Adjacency * adj, struct Graph * graph) {
  adj->dim = graph->num_nodes;
  adj->matrix = malloc(graph->num_nodes * sizeof(int*));
  for (size_t i = 0; i < graph->num_nodes; i++) {
    adj->matrix[i] = malloc(graph->num_nodes * sizeof(int));
    memset(adj->matrix[i], 0, graph->num_nodes * sizeof(int));
  }
  for (size_t i = 0; i < graph->num_edges; i++) {
    adj->matrix[graph->edges[i]->tail->id][graph->edges[i]->head->id] = 1;
  }
}

void create_mapping(struct Mapping * map) {
  size_t limit = min(G1->num_nodes, G2->num_nodes);
  if (limit == G1->num_nodes) {
    map->larger = G2;
    map->smaller = G1;
  } else {
    map->larger = G1;
    map->smaller = G2;
  }
  map->num_mappings = limit;

  // map->tails = malloc(limit * sizeof(struct Node *));
  // map->heads = malloc(limit * sizeof(struct Node *));
  map->translation = trie_new();
  for (size_t i = 0; i < limit; i++) {
    trie_insert(map->translation, map->smaller->nodes[i]->name, map->larger->nodes[i]);
    // map->tails[i] = g1->nodes[i];
    // map->heads[i] = g2->nodes[i];
  }
}

int create_graph(char * filename, struct Graph * graph) {
  char * line = NULL, * token;
  size_t len = 0;
  /* file reading */
  FILE * handle = fopen(filename, "r");
  if (handle == NULL)
    exit(EXIT_FAILURE);
  graph->num_edges = 0;
  graph->num_nodes = 0;
  for (size_t i = 0; i < 5; i++)
    getline(&line, &len, handle);
  sscanf(line, "%d", &graph->num_nodes);
  graph->nodes = malloc(graph->num_nodes * sizeof(struct Node *));
  for (size_t i = 0; i < graph->num_nodes; i++) {
      /* parse node */
      getline(&line, &len, handle);
      token = strtok(line, "{");
      token = strtok(NULL, "}");
      insert_node(graph, i, token);
  }
  /* get number of edges */
  getline(&line, &len, handle);
  sscanf(line, "%d", &graph->num_edges);
  graph->edges = malloc(graph->num_edges * sizeof(struct Edge *));
  for (size_t i = 0; i < graph->num_edges; i++) {
      /* parse edge */
      getline(&line, &len, handle);
      int tail, head;
      sscanf(line, "%d %d", &tail, &head);
      insert_edge(graph, i, tail, head);
  }
  fclose(handle);
  if (line)
      free(line);
  graph->n2e = trie_new();
  for (size_t i = 0; i < graph->num_nodes; i++) {
    char * name = graph->nodes[i]->name;
    trie_insert(graph->n2e, name, arraylist_new(0));
    ArrayList * edges = trie_lookup(graph->n2e, name);
    for (size_t j = 0; j < graph->num_edges; j++) {
      if (graph->edges[j]->tail == graph->nodes[i])
        arraylist_append(edges, graph->edges[j]->head);
    }
  }
}

void create_alignment(struct Alignment * a, char * files[]) {
  struct Mapping * map = malloc(sizeof(struct Mapping));
  G1 = malloc(sizeof(struct Graph));
  G2 = malloc(sizeof(struct Graph));
  struct Adjacency * a1 = malloc(sizeof(struct Adjacency)),
                   * a2 = malloc(sizeof(struct Adjacency));
  create_graph(files[1], G1);
  create_graph(files[2], G2);
  create_mapping(map);
  create_adj(a1, G1);
  create_adj(a2, G2);
  a->map = map;
  a->a1 = a1;
  a->a2 = a2;
  a->score = edge_coverage(a);
}

#endif
