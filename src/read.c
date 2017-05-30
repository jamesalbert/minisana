#include "read.h"

void read_file(char * filename, void (*steps[])(), int num_steps, void * helper) {
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
  };
  for (size_t i = 0; i < num_steps; ++i)
    steps[i](helper);
  fclose(File->handle);
  if (File->line)
      free(File->line);
}
