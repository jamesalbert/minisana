#include "create.h"

void create_adj(struct Adjacency * adj, struct Graph * graph) {
  adj->dim = graph->num_nodes;
  adj->matrix = malloc(adj->dim * sizeof(unsigned int*));
  for (size_t i = 0; i < adj->dim; i++) {
    adj->matrix[i] = malloc(adj->dim * sizeof(unsigned int));
    memset(adj->matrix[i], 0, adj->dim * sizeof(unsigned int));
  }
  for (size_t i = 0; i < graph->num_nodes; i++)
    for (size_t j = 0; j < graph->num_outgoing[i]; j++)
      adj->matrix[i][graph->outgoing[i][j]] = 1;
}

void create_graph(char * filename, struct Graph * graph) {
  char * line = NULL, * token;
  size_t len = 0;
  /* file reading */
  FILE * handle = fopen(filename, "r");
  if (handle == NULL) {
    perror(filename);
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < 5; i++)
    getline(&line, &len, handle);
  sscanf(line, "%u", &graph->num_nodes);
  graph->id2name = malloc(graph->num_nodes * sizeof(char *));
  graph->name2id = malloc(graph->num_nodes * sizeof(int));
  memset(graph->name2id, (unsigned int)-1, graph->num_nodes * sizeof(int));
  for (size_t i = 0; i < graph->num_nodes; i++) {
    /* parse node */
    getline(&line, &len, handle);
    token = strtok(line, "{");
    token = strtok(NULL, "}");
    graph->id2name[i] = malloc(strlen(token) * sizeof(char));
    strcpy(graph->id2name[i], token);
    int hashed = hash(token);
    while (graph->name2id[hashed++ % graph->num_nodes] != (unsigned int)-1);
    graph->name2id[--hashed % graph->num_nodes] = i;
  }
  /* get number of edges */
  getline(&line, &len, handle);
  sscanf(line, "%u", &graph->num_edges);
  graph->outgoing = malloc(graph->num_nodes * sizeof(short int *));
  graph->incoming = malloc(graph->num_nodes * sizeof(short int *));
  graph->num_outgoing = malloc(graph->num_nodes * sizeof(unsigned int *));
  graph->num_incoming = malloc(graph->num_nodes * sizeof(unsigned int *));
  memset(graph->num_outgoing, 0, graph->num_nodes * sizeof(unsigned int *));
  memset(graph->num_incoming, 0, graph->num_nodes * sizeof(unsigned int *));
  for (size_t i = 0; i < graph->num_nodes; i++)
    graph->outgoing[i] = malloc(0);
  for (size_t i = 0; i < graph->num_edges; i++) {
    /* parse edge */
    getline(&line, &len, handle);
    unsigned int tail, head;
    sscanf(line, "%u %u", &tail, &head);
    size_t tail_size = ++graph->num_outgoing[--tail] * sizeof(short int);
    size_t head_size = ++graph->num_incoming[--head] * sizeof(short int);
    graph->outgoing[tail] = realloc(graph->outgoing[tail], tail_size);
    graph->incoming[head] = realloc(graph->incoming[head], head_size);
    graph->outgoing[tail][graph->num_outgoing[tail] - 1] = head;
    graph->incoming[head][graph->num_incoming[head] - 1] = tail;
  }
  fclose(handle);
  if (line)
      free(line);
}

void create_sequence(char * filename) {
  char * line = NULL, * token, * last_node,
       * score_str, * name1, * name2;
  size_t len = 0;
  short int id1 = -1, id2;
  /* file reading */
  FILE * handle = fopen(filename, "r");
  if (handle == NULL) {
    perror("file error");
    exit(EXIT_FAILURE);
  }
  G1->sequence = malloc(G1->num_nodes * sizeof(int *));
  G1->sequence_map = malloc(G1->num_nodes * sizeof(short int *));
  G1->sequence_adj = malloc(G1->num_nodes * sizeof(short int *));
  for (size_t i = 0; i < G1->num_nodes; i++) {
    G1->sequence_adj[i] = malloc(G2->num_nodes * sizeof(short int *));
    memset(G1->sequence_adj[i], 0, G2->num_nodes * sizeof(short int *));
  }
  G1->num_sequences = malloc(G1->num_nodes * sizeof(unsigned int));
  memset(G1->num_sequences, 0, G1->num_nodes * sizeof(unsigned int));
  last_node = malloc(20 * sizeof(char));
  strcpy(last_node, "NULL");
  if (getline(&line, &len, handle) == -1) {
    printf("invalid sequence file\n");
    exit(EXIT_FAILURE);
  } do {
    name1 = strtok(line, "\t");
    name2 = strtok(NULL, "\t");
    if (strcmp(last_node, name1) != 0) {
      strcpy(last_node, name1);
      G1->sequence[++id1] = malloc(
        G2->num_nodes * sizeof(int));
      G1->sequence_map[id1] = malloc(
        G2->num_nodes * sizeof(short int));
    }
    int hashed = hash(name2);
    while (strcmp(name2, G2->id2name[G2->name2id[hashed++ % G2->num_nodes]]) != 0);
    id2 = G2->name2id[--hashed % G2->num_nodes];
    score_str = strtok(NULL, "\n");
    G1->sequence[id1][id2] = (int)(atof(score_str) * 10000);
    G1->sequence_map[id1][G1->num_sequences[id1]++] = id2;
    G1->sequence_adj[id1][id2] = 1;
  } while (getline(&line, &len, handle) != -1);
  fclose(handle);
  free(last_node);
  if (line)
      free(line);
}

void create_alignment(char * files[], double alpha) {
  A1 = malloc(sizeof(struct Alignment));
  A2 = malloc(sizeof(struct Alignment));
  G1 = malloc(sizeof(struct Graph));
  G2 = malloc(sizeof(struct Graph));
  A->alpha = alpha;
  printf("reading topology files...\n");
  create_graph(files[0], G1);
  create_graph(files[1], G2);
  if (G1->num_nodes > G2->num_nodes) {
    struct Graph * temp = G1;
    G1 = G2;
    G2 = temp;
  }
  printf("creating adjacency matrices...\n");
  create_adj(A1, G1);
  create_adj(A2, G2);
  G1->translate = malloc(G1->num_nodes * sizeof(short int));
  G2->taken = malloc(G2->num_nodes * sizeof(short int));
  memset(G2->taken, 0, G2->num_nodes * sizeof(short int));
  for (size_t i = 0; i < G1->num_nodes; i++) {
    G1->translate[i] = i;
    G2->taken[i] = 1;
  }
  printf("reading sequences...\n");
  create_sequence(files[2]);
  full_edge_coverage();
  full_sequence_similarity();
  update_score();
  printf("\n\n");
}
