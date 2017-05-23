#ifndef PRINT_H
#define PRINT_H

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
  printf("\033[A\r@@@ Generated at temp: %f, time: %d, score: %f, edges aligned: %.0f / %d @@@\n",
  t, i, A->score, A->score * G1->num_edges, G1->num_edges);
}

#endif
