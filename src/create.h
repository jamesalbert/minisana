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

void create_mapping(struct Mapping * map, struct Graph * g1, struct Graph * g2) {
  size_t limit = min(g1->num_nodes, g2->num_nodes);
  if (limit == g1->num_edges) {
    map->larger = g2;
    map->smaller = g1;
  } else {
    map->larger = g1;
    map->smaller = g2;
  }
  map->num_mappings = limit;
  map->tails = malloc(limit * sizeof(struct Node *));
  map->heads = malloc(limit * sizeof(struct Node *));
  for (size_t i = 0; i < limit; i++) {
    map->tails[i] = g1->nodes[i];
    map->heads[i] = g2->nodes[i];
  }
}

int create_graph(char * filename, struct Graph * graph) {
  char * line = NULL, * token;
  size_t len = 0;
  size_t read;
  int line_num = 1;
  /* regex */
  regex_t node_regex, edge_regex;
  regcomp(&node_regex, "^|{[[:alnum:]]", 0);
  regcomp(&edge_regex, ".* ", 0);
  /* file reading */
  FILE * handle = fopen(filename, "r");
  if (handle == NULL)
    exit(EXIT_FAILURE);
  graph->num_edges = 0;
  graph->num_nodes = 0;
  while ((read = getline(&line, &len, handle)) != -1) {
    /* skip first 4 lines */
    if (line_num < 5) {
      line_num++;
      continue;
    }
    /* get node count from 5th line */
    else if (line_num == 5) {
      /* get number of nodes */
      int size;
      sscanf(line, "%d", &size);
      graph->nodes = malloc(size * sizeof(struct Node *));
      line_num++;
      continue;
    }
    if (regexec(&node_regex, line, 0, NULL, 0)) {
      if (regexec(&edge_regex, line, 0, NULL, 0)) {
        /* get number of edges */
        int size;
        sscanf(line, "%d", &size);
        graph->edges = malloc(size * sizeof(struct Edge *));
        continue;
      }
      /* parse edge */
      int tail, head;
      sscanf(line, "%d %d", &tail, &head);
      insert_edge(graph, tail, head);
      continue;
    }
    /* parse node */
    strtok(line, "{");
    insert_node(graph, strtok(NULL, "}"));
  }
  regfree(&node_regex);
  regfree(&edge_regex);
  fclose(handle);
  if (line)
      free(line);
}

void create_alignment(struct Alignment * a, char * files[]) {
  struct Mapping * map = malloc(sizeof(struct Mapping));
  struct Graph * g1 = malloc(sizeof(struct Graph)),
               * g2 = malloc(sizeof(struct Graph));
  struct Adjacency * a1 = malloc(sizeof(struct Adjacency)),
                   * a2 = malloc(sizeof(struct Adjacency));
  create_graph(files[1], g1);
  create_graph(files[2], g2);
  create_mapping(map, g1, g2);
  create_adj(a1, g1);
  create_adj(a2, g2);
  a->map = map;
  a->a1 = a1;
  a->a2 = a2;
}

#endif
