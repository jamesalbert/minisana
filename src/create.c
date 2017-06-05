#include "create.h"

Graph_t * new_graph() {
  return malloc(sizeof(Graph_t));
}

Alignment_t * new_alignment() {
  return malloc(sizeof(Alignment_t));
}

MiniMan_t * new_miniman() {
  MiniMan_t * mm = malloc(sizeof(MiniMan_t));
  mm->A = new_alignment();
  mm->G1 = new_graph();
  mm->G2 = new_graph();
  return mm;
}

short ** create_adj(Graph_t * graph) {
  // adj->dim = graph->num_nodes;
  short ** adj = malloc(graph->num_nodes * sizeof(short*));
  size_t i, j;
  for (i = 0; i < graph->num_nodes; i++) {
    adj[i] = malloc(graph->num_nodes * sizeof(short));
    memset(adj[i], 0, graph->num_nodes * sizeof(short));
  }
  for (i = 0; i < graph->num_nodes; i++)
    for (j = 0; j < graph->num_outgoing[i]; j++)
      adj[i][graph->outgoing[i][j]] = 1;
  return adj;
}

void ignore_header(Graph_t * graph) {
  size_t i;
  for (i = 0; i < 5; i++)
    getline(&File->line, &File->len, File->handle);
}

void read_num_nodes(Graph_t * graph) {
  sscanf(File->line, "%u", &graph->num_nodes);
  graph->id2name = malloc(graph->num_nodes * sizeof(char *));
  graph->name2id = malloc(graph->num_nodes * sizeof(int));
  memset(graph->name2id, -1, graph->num_nodes * sizeof(int));
}

void read_nodes(Graph_t * graph) {
  char * token;
  short id;
  for (id = 0; id < graph->num_nodes; id++) {
    getline(&File->line, &File->len, File->handle);
    token = strtok(File->line, "{");
    token = strtok(NULL, "}");
    graph->id2name[id] = malloc(
      strlen(token) * sizeof(char));
    strcpy(graph->id2name[id], token);
    associate_name(token, id, graph);
  }
}

void read_num_edges(Graph_t * graph) {
  getline(&File->line, &File->len, File->handle);
  sscanf(File->line, "%u", &graph->num_edges);
  graph->outgoing = malloc(
    graph->num_nodes * sizeof(short int *));
  graph->incoming = malloc(
    graph->num_nodes * sizeof(short int *));
  graph->num_outgoing = malloc(
    graph->num_nodes * sizeof(unsigned int *));
  graph->num_incoming = malloc(
    graph->num_nodes * sizeof(unsigned int *));
  memset(graph->num_outgoing, 0,
         graph->num_nodes * sizeof(unsigned int *));
  memset(graph->num_incoming, 0,
         graph->num_nodes * sizeof(unsigned int *));
  size_t i;
  for (i = 0; i < graph->num_nodes; i++)
    graph->outgoing[i] = malloc(0);
}

void read_edges(Graph_t * graph) {
  size_t i;
  for (i = 0; i < graph->num_edges; i++) {
    getline(&File->line, &File->len, File->handle);
    unsigned int tail, head;
    sscanf(File->line, "%u %u", &tail, &head);
    size_t tail_size = ++graph->num_outgoing[--tail] * sizeof(short int);
    size_t head_size = ++graph->num_incoming[--head] * sizeof(short int);
    graph->outgoing[tail] = realloc(
      graph->outgoing[tail], tail_size);
    graph->incoming[head] = realloc(
      graph->incoming[head], head_size);
    graph->outgoing[tail][graph->num_outgoing[tail] - 1] = head;
    graph->incoming[head][graph->num_incoming[head] - 1] = tail;
  }
}

void init_lists(MiniMan_t * mm) {
  mm->G1->sequence = malloc(mm->G1->num_nodes * sizeof(double *));
  mm->G1->sequence_map = malloc(mm->G1->num_nodes * sizeof(short int *));
  mm->G1->sequence_adj = malloc(mm->G1->num_nodes * sizeof(short int *));
  size_t i;
  for (i = 0; i < mm->G1->num_nodes; i++) {
    mm->G1->sequence_adj[i] = malloc(mm->G2->num_nodes * sizeof(short int *));
    memset(mm->G1->sequence_adj[i], 0, mm->G2->num_nodes * sizeof(short int *));
  }
  mm->G1->num_sequences = malloc(mm->G1->num_nodes * sizeof(unsigned int));
  memset(mm->G1->num_sequences, 0, mm->G1->num_nodes * sizeof(unsigned int));
}

void validate_file(MiniMan_t * mm) {
  if (getline(&File->line, &File->len, File->handle) == -1) {
    printf("invalid sequence file\n");
    exit(EXIT_FAILURE);
  }
}

void read_sequences(MiniMan_t * mm) {
  short id1 = -1, id2;
  char * last_node = malloc(20 * sizeof(char)),
       * name1, * name2;
  strcpy(last_node, "NULL");
  do {
    name1 = strtok(File->line, "\t");
    name2 = strtok(NULL, "\t");
    if (strcmp(last_node, name1) != 0) {
      strcpy(last_node, name1);
      mm->G1->sequence[++id1] = malloc(
        mm->G2->num_nodes * sizeof(double));
      mm->G1->sequence_map[id1] = malloc(
        mm->G2->num_nodes * sizeof(short int));
      memset(mm->G1->sequence[id1], 0.0, mm->G2->num_nodes * sizeof(double));
      memset(mm->G1->sequence_map[id1], -1, mm->G2->num_nodes * sizeof(short int));
    }
    id2 = name2id(name2, mm->G2);
    char * score_str = strtok(NULL, "\n");
    mm->G1->sequence[id1][id2] = atof(score_str);
    mm->G1->sequence_map[id1][mm->G1->num_sequences[id1]++] = id2;
    mm->G1->sequence_adj[id1][id2] = 1;
  } while (getline(&File->line, &File->len, File->handle) != -1);
  free(last_node);
}

void create_graph(char * filename, Graph_t * graph) {
  void (*steps[5])();
  steps[0] = ignore_header;
  steps[1] = read_num_nodes;
  steps[2] = read_nodes;
  steps[3] = read_num_edges;
  steps[4] = read_edges;
  if (read_file(filename, steps, 5, graph) == 1)
    exit(EXIT_FAILURE);
}

void create_sequence(MiniMan_t * mm) {
  void (*steps[3])();
  steps[0] = init_lists;
  steps[1] = validate_file;
  steps[2] = read_sequences;
  if (read_file(mm->seq_file, steps, 3, mm) == 1)
    exit(EXIT_FAILURE);
}

void create_alignment(MiniMan_t * mm) {
  create_graph(mm->g1_file, mm->G1);
  create_graph(mm->g2_file, mm->G2);
  if (mm->G1->num_nodes > mm->G2->num_nodes) {
    Graph_t * temp = mm->G1;
    mm->G1 = mm->G2;
    mm->G2 = temp;
  }
  // create_adj(mm->A1, mm->G1);
  // create_adj(mm->A2, mm->G2);
  mm->adjmat1 = create_adj(mm->G1);
  mm->adjmat2 = create_adj(mm->G2);
  mm->G1->translate = malloc(mm->G1->num_nodes * sizeof(short int));
  mm->G2->taken = malloc(mm->G2->num_nodes * sizeof(short int));
  memset(mm->G2->taken, 0, mm->G2->num_nodes * sizeof(short int));
  size_t i;
  for (i = 0; i < mm->G1->num_nodes; i++) {
    mm->G1->translate[i] = i;
    mm->G2->taken[i] = 1;
  }
  create_sequence(mm);
  full_edge_coverage(mm);
  full_sequence_similarity(mm);
  update_score(mm);
  printf("\n\n");
}
