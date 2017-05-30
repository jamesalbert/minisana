#include "print.h"

void print_mapping(MiniMan_t * mm) {
  FILE * json_file = fopen(mm->output_file, "w");
  fprintf(json_file, "{\n");
  for (size_t i = 0; i < mm->G1->num_nodes; i++) {
    char * name = mm->G1->id2name[i],
         * aname = mm->G2->id2name[mm->G1->translate[i]];
    fprintf(json_file, "\t\"%s\": \"%s\"", name, aname);
    if (i != mm->G1->num_nodes - 1)
      fprintf(json_file, ",\n");
  }
  fprintf(json_file, "\n}\n");
  fclose(json_file);
}

void print_status(MiniMan_t * mm, double t, int i) {
  printf("\033[A\r@@@ Generated at temp: %f, time: %d, score: %f, sequence similarity: %f / %d, edges aligned: %u / %u @@@\n",
  t, i, mm->A->score, mm->A->sequence_score, mm->G2->num_nodes, mm->A->topology_score, mm->G1->num_edges);
}

void print_node(MiniMan_t * mm, short int node, bool stop) {
  printf("node %hu (%s) has %u edges:\n", node, mm->G1->id2name[node], mm->G1->num_outgoing[node]);
  for (size_t i = 0; i < mm->G1->num_outgoing[node]; i++)
    printf("\\_>%d (%s)\n", mm->G1->outgoing[node][i], mm->G1->id2name[mm->G1->outgoing[node][i]]);
  printf("\n\n");
  if (stop)
    getchar();
}

void print_sequences(MiniMan_t * mm) {
  double score = 0.0;
  for (short int i = 0; i < mm->G1->num_nodes; i++) {
    if (mm->G1->sequence[i] != NULL) {
      short int ti = mm->G1->translate[i];
      printf("  %f\n+ %f = %f\n", score, mm->G1->sequence[i][ti], score + mm->G1->sequence[i][ti]);
      score += mm->G1->sequence[i][ti];
    }
  }
  printf("with a total of %f\n", score);
}
