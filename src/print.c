#include "print.h"

void print_mapping() {
  FILE * json_file = fopen("mini.out.json", "w");
  fprintf(json_file, "{\n");
  for (size_t i = 0; i < G1->num_nodes; i++) {
    char * name = G1->id2name[i],
         * aname = G2->id2name[G1->translate[i]];
    fprintf(json_file, "\t\"%s\": \"%s\"", name, aname);
    if (i != G1->num_nodes - 1)
      fprintf(json_file, ",\n");
  }
  fprintf(json_file, "\n}\n");
  fclose(json_file);
}

void print_status(double t, int i) {
  printf("\033[A\r@@@ Generated at temp: %f, time: %d, score: %f, sequence similarity: %u, edges aligned: %u / %u @@@\n",
  t, i, A->score, A->sequence_score, A->topology_score, G1->num_edges);
}

void print_node(int node, bool stop) {
  // printf("node %hu (%s) has %u edges with a coverage of %u:\n", node, G1->id2name[node], G1->num_outgoing[node], edge_coverage(node));
  for (size_t i = 0; i < G1->num_outgoing[node]; i++)
    printf("\\_>%d (%s)\n", G1->outgoing[node][i], G1->id2name[G1->outgoing[node][i]]);
  printf("\n\n");
  if (stop)
    getchar();
}

void print_sequences() {
  for (short int i = 0; i < G1->num_nodes; i++) {
    printf("%d (%s):\n", i, G1->id2name[i]);
    for (int j = 0; j < G1->num_sequences[i]; j++) {
      short int id = G1->sequence_map[i][j];
      printf("\t\\_> %d (%s) => %d\n", id, G2->id2name[id], G1->sequence[i][id]);
    }
  }
}
