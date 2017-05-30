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

void ignore_header() {
  for (size_t i = 0; i < 5; i++)
    getline(&File->line, &File->len, File->handle);
}

void read_num_nodes() {
  sscanf(File->line, "%u", &File->graph->num_nodes);
  File->graph->id2name = malloc(File->graph->num_nodes * sizeof(char *));
  File->graph->name2id = malloc(File->graph->num_nodes * sizeof(int));
  memset(File->graph->name2id, -1, File->graph->num_nodes * sizeof(int));
}

void read_nodes() {
  char * token;
  for (short int id = 0; id < File->graph->num_nodes; id++) {
    getline(&File->line, &File->len, File->handle);
    token = strtok(File->line, "{");
    token = strtok(NULL, "}");
    File->graph->id2name[id] = malloc(
      strlen(token) * sizeof(char));
    strcpy(File->graph->id2name[id], token);
    associate_name(token, id, File->graph);
  }
}

void read_num_edges() {
  getline(&File->line, &File->len, File->handle);
  sscanf(File->line, "%u", &File->graph->num_edges);
  File->graph->outgoing = malloc(
    File->graph->num_nodes * sizeof(short int *));
  File->graph->incoming = malloc(
    File->graph->num_nodes * sizeof(short int *));
  File->graph->num_outgoing = malloc(
    File->graph->num_nodes * sizeof(unsigned int *));
  File->graph->num_incoming = malloc(
    File->graph->num_nodes * sizeof(unsigned int *));
  memset(File->graph->num_outgoing, 0,
         File->graph->num_nodes * sizeof(unsigned int *));
  memset(File->graph->num_incoming, 0,
         File->graph->num_nodes * sizeof(unsigned int *));
  for (size_t i = 0; i < File->graph->num_nodes; i++)
    File->graph->outgoing[i] = malloc(0);
}

void read_edges() {
  for (size_t i = 0; i < File->graph->num_edges; i++) {
    getline(&File->line, &File->len, File->handle);
    unsigned int tail, head;
    sscanf(File->line, "%u %u", &tail, &head);
    size_t tail_size = ++File->graph->num_outgoing[--tail] * sizeof(short int);
    size_t head_size = ++File->graph->num_incoming[--head] * sizeof(short int);
    File->graph->outgoing[tail] = realloc(
      File->graph->outgoing[tail], tail_size);
    File->graph->incoming[head] = realloc(
      File->graph->incoming[head], head_size);
    File->graph->outgoing[tail][File->graph->num_outgoing[tail] - 1] = head;
    File->graph->incoming[head][File->graph->num_incoming[head] - 1] = tail;
  }
}

void init_lists() {
  G1->sequence = malloc(G1->num_nodes * sizeof(double *));
  G1->sequence_map = malloc(G1->num_nodes * sizeof(short int *));
  G1->sequence_adj = malloc(G1->num_nodes * sizeof(short int *));
  for (size_t i = 0; i < G1->num_nodes; i++) {
    G1->sequence_adj[i] = malloc(G2->num_nodes * sizeof(short int *));
    memset(G1->sequence_adj[i], 0, G2->num_nodes * sizeof(short int *));
  }
  G1->num_sequences = malloc(G1->num_nodes * sizeof(unsigned int));
  memset(G1->num_sequences, 0, G1->num_nodes * sizeof(unsigned int));
}

void validate_file() {
  if (getline(&File->line, &File->len, File->handle) == -1) {
    printf("invalid sequence file\n");
    exit(EXIT_FAILURE);
  }
}

void read_sequences() {
  short int id1 = -1, id2;
  char * last_node = malloc(20 * sizeof(char)),
       * name1, * name2;
  strcpy(last_node, "NULL");
  do {
    name1 = strtok(File->line, "\t");
    name2 = strtok(NULL, "\t");
    if (strcmp(last_node, name1) != 0) {
      strcpy(last_node, name1);
      G1->sequence[++id1] = malloc(
        G2->num_nodes * sizeof(double));
      G1->sequence_map[id1] = malloc(
        G2->num_nodes * sizeof(short int));
      memset(G1->sequence[id1], 0.0, G2->num_nodes * sizeof(double));
      memset(G1->sequence_map[id1], -1, G2->num_nodes * sizeof(short int));
    }
    id2 = name2id(name2, G2);
    char * score_str = strtok(NULL, "\n");
    G1->sequence[id1][id2] = atof(score_str);
    G1->sequence_map[id1][G1->num_sequences[id1]++] = id2;
    G1->sequence_adj[id1][id2] = 1;
  } while (getline(&File->line, &File->len, File->handle) != -1);
  free(last_node);
}

void create_graph(char * filename, struct Graph * graph) {
  void (*steps[5])();
  steps[0] = ignore_header;
  steps[1] = read_num_nodes;
  steps[2] = read_nodes;
  steps[3] = read_num_edges;
  steps[4] = read_edges;
  read_file(filename, steps, 5, graph);
}

void create_sequence(char * filename) {
  void (*steps[3])();
  steps[0] = init_lists;
  steps[1] = validate_file;
  steps[2] = read_sequences;
  read_file(filename, steps, 3, NULL);
}

void create_alignment(struct Alignment * A, struct MiniMan * mm) {
  A1 = malloc(sizeof(struct Alignment));
  A2 = malloc(sizeof(struct Alignment));
  G1 = malloc(sizeof(struct Graph));
  G2 = malloc(sizeof(struct Graph));
  printf("reading topology files...\n");
  create_graph(mm->g1_file, G1);
  create_graph(mm->g2_file, G2);
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
  create_sequence(mm->seq_file);
  full_edge_coverage(A);
  full_sequence_similarity(A);
  update_score(A, mm);
  printf("\n\n");
}
