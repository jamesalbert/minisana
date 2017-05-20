#ifndef PRINT_H
#define PRINT_H

void print_adj(struct Adjacency * adj) {
  for (size_t i = 0; i < adj->dim; i++) {
    for (size_t j = 0; j < adj->dim; j++)
      printf("%d ", adj->matrix[i][j]);
    printf("\n");
  }
}

#endif
