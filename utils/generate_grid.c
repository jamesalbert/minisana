#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char * argv[]) {
  // usage: generate_grid <rows> <cols> <#nodes>
  srand(time(NULL));
  int rows = atoi(argv[1]),
      cols = atoi(argv[2]),
      nodes = atoi(argv[3]);
  int grid[rows][cols];
  printf("%d %d\n", rows, cols);
  for (size_t i = 0; i < rows; i++)
    for (size_t j = 0; j < cols; j++)
      grid[i][j] = 0;
  for (size_t i = 0; i < nodes; i++) {
    int x = rand() % cols, y = rand() % rows;
    grid[x][y] = 1;
  }
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++)
      printf("%d ", grid[i][j]);
    printf("\n");
  }
  return 0;
}
