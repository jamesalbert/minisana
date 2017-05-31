#include "read.h"

short int read_file(char * filename, void (*steps[])(), int num_steps, void * helper) {
  /* file reading */
  FILE * handle = fopen(filename, "r");
  if (handle == NULL) {
    perror(filename);
    return 1;
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
  return 0;
}
