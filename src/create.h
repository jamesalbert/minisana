#ifndef CREATE_H
#define CREATE_H

void create_adj(struct Adjacency * adj, struct Graph * graph) {
  adj->dim = graph->num_nodes;
  adj->matrix = malloc(graph->num_nodes * sizeof(int*));
  for (size_t i = 0; i < graph->num_nodes; i++) {
    adj->matrix[i] = malloc(graph->num_nodes * sizeof(int));
    memset(adj->matrix[i], 0, graph->num_nodes * sizeof(int));
  }
  for (size_t i = 0; i < graph->num_edges; i++)
    adj->matrix[graph->edges[i]->tail->id][graph->edges[i]->head->id] = 1;
}

void create_mapping(Trie * map) {
  size_t limit = G1->num_nodes;
  for (size_t i = 0; i < limit; i++)
    trie_insert(map, G1->nodes[i]->name, G2->nodes[i]);
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
  Trie * map = trie_new();
  A1 = malloc(sizeof(struct Alignment));
  A2 = malloc(sizeof(struct Alignment));
  G1 = malloc(sizeof(struct Graph));
  G2 = malloc(sizeof(struct Graph));
  // file size not a good indication
  // of node/edge count. Files must
  // be specified in order small -> big
  // struct stat st1, st2;
  // stat(files[1], &st1);
  // stat(files[2], &st2);
  // if (st1.st_size > st2.st_size) {
  //   char * temp = files[1];
  //   files[1] = files[2];
  //   files[2] = temp;
  // }
  create_graph(files[1], G1);
  create_graph(files[2], G2);
  create_mapping(map);
  create_adj(A1, G1);
  create_adj(A2, G2);
  a->map = map;
  a->score = full_edge_coverage(a);
}

#endif
