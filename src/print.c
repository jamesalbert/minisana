#include "print.h"

void print_mapping(struct MiniMan * mm) {
  FILE * json_file = fopen(mm->output_file, "w");
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

void print_status(struct Alignment * A, double t, int i) {
  printf("\033[A\r@@@ Generated at temp: %f, time: %d, score: %f, sequence similarity: %f / %d, edges aligned: %u / %u @@@\n",
  t, i, A->score, A->sequence_score, G2->num_nodes, A->topology_score, G1->num_edges);
}

void print_node(short int node, bool stop) {
  printf("node %hu (%s) has %u edges:\n", node, G1->id2name[node], G1->num_outgoing[node]);
  for (size_t i = 0; i < G1->num_outgoing[node]; i++)
    printf("\\_>%d (%s)\n", G1->outgoing[node][i], G1->id2name[G1->outgoing[node][i]]);
  printf("\n\n");
  if (stop)
    getchar();
}

void print_sequences() {
  double score = 0.0;
  for (short int i = 0; i < G1->num_nodes; i++) {
    if (G1->sequence[i] != NULL) {
      short int ti = G1->translate[i];
      printf("  %f\n+ %f = %f\n", score, G1->sequence[i][ti], score + G1->sequence[i][ti]);
      score += G1->sequence[i][ti];
    }
  }
  printf("with a total of %f\n", score);
}
