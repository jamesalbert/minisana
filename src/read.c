#include "read.h"

void read_file(char * filename, void (*steps[])(), int num_steps, struct Graph * graph) {
  /* file reading */
  FILE * handle = fopen(filename, "r");
  if (handle == NULL) {
    perror(filename);
    exit(EXIT_FAILURE);
  }
  File = &(struct ConfigFile) {
    .handle = handle,
    .len = 0,
    .line = NULL,
    .graph = graph
  };
  for (size_t i = 0; i < num_steps; ++i)
    steps[i]();
  fclose(File->handle);
  if (File->line)
      free(File->line);
}
