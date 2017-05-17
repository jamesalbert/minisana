#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define COLS 7
#define ROWS 7
#define SIZE COLS * ROWS * sizeof(int)

int blank_net[COLS][ROWS] = {0};

struct Alignment {
  int net[7][7];   // adjacency matrix representation of network
  int num_coords;  // number of coordinates
  int coords[4][2]; // coordinates of each node
};

void generate_net(struct Alignment * alignment, int coords[4][2], int num_coords) {
  int i;
  int net[7][7] = {0};
  for (i = 0; i < num_coords; i++)
    net[coords[i][0]][coords[i][1]] = 1;
  memcpy(&alignment->net[0][0], &net[0][0], SIZE);
  memcpy(&alignment->coords[0][0], &coords[0][0], num_coords * 2 * sizeof(int));
  alignment->num_coords = num_coords;
}

void print_net(struct Alignment * alignment) {
  int i, j;
  for (i = 0; i < COLS; i++) {
    for (j = 0; j < ROWS; j++)
      printf("%d ", alignment->net[i][j]);
    printf("\n");
  }
}

struct Alignment * swap(struct Alignment * alignment, int n1, int n2) {
  struct Alignment * new_alignment = malloc(sizeof(struct Alignment));
  int size = alignment->num_coords * 2 * sizeof(int);
  new_alignment->num_coords = alignment->num_coords;
  memcpy(&new_alignment->coords[0][0], &alignment->coords[0][0], size);
  memcpy(&new_alignment->coords[n1][0], &alignment->coords[n2][0], 2 * sizeof(int));
  memcpy(&new_alignment->coords[n2][0], &alignment->coords[n1][0], 2 * sizeof(int));
  generate_net(new_alignment, new_alignment->coords, 4);
  return new_alignment;
}

struct Alignment * move(struct Alignment * alignment, int node, int coords[2]) {
  struct Alignment * new_alignment = malloc(sizeof(struct Alignment));
  int size = alignment->num_coords * 2 * sizeof(int);
  new_alignment->num_coords = alignment->num_coords;
  memcpy(&new_alignment->coords[0][0], &alignment->coords[0][0], size);
  memcpy(&new_alignment->coords[node][0], &coords[0], 2 * sizeof(int));
  generate_net(new_alignment, new_alignment->coords, 4);
  return new_alignment;
}

int get_node_by_coords(struct Alignment * alignment, int coords[2]) {
  int i;
  for (i = 0; i < alignment->num_coords; i++)
    if (alignment->coords[i][0] == coords[0] && alignment->coords[i][1] == coords[1])
      return i;
  return -1;
}

void get_neighbors(struct Alignment * alignment, struct Alignment ** neighbors, int * num_neighbors) {
  int i, j, k;
  *num_neighbors = 0;
  for (i = 0; i < alignment->num_coords; i++)
    for (j = 0; j < alignment->num_coords; j++)
      if (i != j)
        neighbors[(*num_neighbors)++] = swap(alignment, i, j);
  for (i = 0; i < alignment->num_coords; i++)
    for (j = 0; j < ROWS; j++)
      for (k = 0; k < COLS; k++)
        if (alignment->net[j][k] == 0) {
          int coords[2] = {j, k};
          neighbors[(*num_neighbors)++] = move(alignment, i, coords);
        }
}

double edge_cover(struct Alignment * alignment, struct Alignment * solution) {
  int i, j;
  double passed = 0.0;
  for (i = 0; i < alignment->num_coords; i++)
    for (j = 0; j < solution->num_coords; j++) {
      if (alignment->coords[i][0] == solution->coords[j][0] &&
          alignment->coords[i][1] == solution->coords[j][1])
        passed++;
    }
  return passed / 4.0;
}

struct Alignment * get_best_neighbor(struct Alignment * alignment, struct Alignment * solution) {
  struct Alignment ** neighbors = malloc(sizeof(struct Alignment) * 1000);
  struct Alignment * best_neighbor;
  int i = 0, num_neighbors;
  double best_score = 0;
  get_neighbors(alignment, neighbors, &num_neighbors);
  for (i = 0; i < num_neighbors; i++) {
    double score = edge_cover(neighbors[i], solution);
    if (score >= best_score) {
      best_score = score;
      best_neighbor = neighbors[i];
    }
  }
  memcpy(best_neighbor, best_neighbor, sizeof(best_neighbor));
  free(neighbors);
  return best_neighbor;
}

double probability(double es, double es_new, double t) {
  return exp(-(es_new - es) / t);
}

double temperature(double k) {
  return 1 * exp(-1 * (k / 100.0));
}

int main(int argc, char * argv[]) {
  int i;
  double t, es, es_new, p;
  bool accept;
  int sol_coords[4][2] = {
    {3, 4},
    {3, 6},
    {5, 4},
    {5, 6}
  };
  int s_coords[4][2] = {
    {0, 0},
    {3, 5},
    {4, 1},
    {5, 2}
  };
  struct Alignment * s = malloc(sizeof(struct Alignment)),
                   * sol = malloc(sizeof(struct Alignment)),
                   * s_new;
  generate_net(sol, sol_coords, 4);
  generate_net(s, s_coords, 4);
  printf("starting with:\n");
  print_net(s);
  printf("\ntrying to find:\n");
  print_net(sol);
  for (i = 0; i < 1000; i++) {
    s_new = get_best_neighbor(s, sol);
    t = temperature(i);
    es = edge_cover(s, sol);
    es_new = edge_cover(s_new, sol);
    p = probability(es, es_new, t);
    if (es_new - es >= 0)
      s = s_new;
    else {
      accept = ! ((rand() % 100) < p);
      if (accept)
        s = s_new;
    }
  }
  printf("\nfound:\n");
  print_net(s);
  free(s);
  free(sol);
  exit(EXIT_SUCCESS);
}
