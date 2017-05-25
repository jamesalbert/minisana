#ifndef CREATE_H
#define CREATE_H

void create_adj(struct Adjacency * adj, struct Graph * graph) {
  adj->dim = graph->num_nodes;
  adj->matrix = malloc(adj->dim * sizeof(int*));
  for (size_t i = 0; i < adj->dim; i++) {
    adj->matrix[i] = malloc(adj->dim * sizeof(int));
    memset(adj->matrix[i], 0, adj->dim * sizeof(int));
  }
  for (size_t i = 0; i < graph->num_nodes; i++)
    for (size_t j = 0; j < graph->num_outgoing[i]; j++)
      adj->matrix[i][graph->outgoing_edges[i][j]] = 1;
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
  sscanf(line, "%u", &graph->num_nodes);
  graph->id2name = malloc(graph->num_nodes * sizeof(char *));
  for (size_t i = 0; i < graph->num_nodes; i++) {
    /* parse node */
    getline(&line, &len, handle);
    token = strtok(line, "{");
    token = strtok(NULL, "}");
    graph->id2name[i] = malloc(strlen(token) * sizeof(char));
    strcpy(graph->id2name[i], token);
  }
  /* get number of edges */
  getline(&line, &len, handle);
  sscanf(line, "%d", &graph->num_edges);
  graph->outgoing_edges = malloc(graph->num_nodes * sizeof(int *));
  graph->incoming_edges = malloc(graph->num_nodes * sizeof(int *));
  graph->num_outgoing = malloc(graph->num_nodes * sizeof(int *));
  graph->num_incoming = malloc(graph->num_nodes * sizeof(int *));
  memset(graph->num_outgoing, 0, graph->num_nodes * sizeof(int *));
  memset(graph->num_incoming, 0, graph->num_nodes * sizeof(int *));
  for (size_t i = 0; i < graph->num_nodes; i++)
    graph->outgoing_edges[i] = malloc(0);
  for (size_t i = 0; i < graph->num_edges; i++) {
    /* parse edge */
    getline(&line, &len, handle);
    int tail, head;
    sscanf(line, "%d %d", &tail, &head);
    size_t tail_size = ++graph->num_outgoing[--tail] * sizeof(int);
    size_t head_size = ++graph->num_incoming[--head] * sizeof(int);
    graph->outgoing_edges[tail] = realloc(graph->outgoing_edges[tail], tail_size);
    graph->incoming_edges[head] = realloc(graph->incoming_edges[head], head_size);
    graph->outgoing_edges[tail][graph->num_outgoing[tail] - 1] = head;
    graph->incoming_edges[head][graph->num_incoming[head] - 1] = tail;
  }
  fclose(handle);
  if (line)
      free(line);
}

void create_alignment(char * files[]) {
  A1 = malloc(sizeof(struct Alignment));
  A2 = malloc(sizeof(struct Alignment));
  G1 = malloc(sizeof(struct Graph));
  G2 = malloc(sizeof(struct Graph));
  create_graph(files[1], G1);
  create_graph(files[2], G2);
  if (G1->num_nodes > G2->num_nodes) {
    struct Graph * temp = G1;
    G1 = G2;
    G2 = temp;
  }
  create_adj(A1, G1);
  create_adj(A2, G2);
  G1->translate = malloc(G1->num_nodes * sizeof(int));
  G2->taken = malloc(G2->num_nodes * sizeof(int));
  memset(G2->taken, 0, G2->num_nodes * sizeof(int));
  for (int i = 0; i < G1->num_nodes; i++) {
    G1->translate[i] = i;
    G2->taken[i] = 1;
  }
  A->score = full_edge_coverage();
}

#endif
