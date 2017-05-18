#define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <term.h>


struct Alignment {
  int ** net;   // adjacency matrix representation of network
  int ** coords; // coordinates of each node
  int num_coords;  // number of coordinates
};

struct Network {
  int ** net;
  int rows;
  int cols;
  int size;
} network;

void init_network(int rows, int cols) {
  network.rows = rows;
  network.cols = cols;
  network.size = (rows * sizeof(int*)) * (cols * sizeof(int));
  network.net = malloc(rows * sizeof(int*));
  for (int i = 0; i < rows; i++) {
    network.net[i] = malloc(cols * sizeof(int));
    for (int j = 0; j < cols; j++) {
      network.net[i][j] = 0;
    }
  }
}

void print_net(int ** net, int rows, int cols) {
  int i, j;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++)
      printf("%d ", net[i][j]);
    printf("\n");
  }
}

void print_coords(int ** coords, int num_coords) {
  printf("{");
  for (int i = 0; i < num_coords; i++)
    printf("\n\t[ %d, %d ]", coords[i][0], coords[i][1]);
  printf("\n}\n");
}

void generate_net(struct Alignment * alignment, int ** coords, int num_coords) {
  alignment->net = malloc(network.rows * sizeof(int*));
  alignment->net[0] = malloc(network.rows * network.cols * sizeof(network.net[0][0]));
  for (size_t i = 0; i < network.rows; i++) {
    alignment->net[i] = malloc(network.cols * sizeof(int)); //alignment->net[i-1] + network.cols;
    for (size_t j = 0; j < network.cols; j++) {
      alignment->net[i][j] = 0;
    }
  }
  for (int i = 0; i < num_coords; i++) {
    alignment->net[coords[i][0]][coords[i][1]] = 1;
  }
  memcpy(&alignment->coords, &coords, sizeof(coords));
  alignment->num_coords = num_coords;
}

struct Alignment * swap(struct Alignment * alignment, int n1, int n2) {
  struct Alignment * new_alignment = malloc(sizeof(struct Alignment));
  new_alignment->num_coords = alignment->num_coords;
  // allocate coords
  new_alignment->coords = malloc(alignment->num_coords * sizeof(int*));
  // copy coords
  for (size_t i = 0; i < alignment->num_coords; i++) {
    new_alignment->coords[i] = malloc(2 * sizeof(int));
    new_alignment->coords[i][0] = alignment->coords[i][0];
    new_alignment->coords[i][1] = alignment->coords[i][1];
  }
  // swap coords
  int n10 = new_alignment->coords[n1][0];
  int n11 = new_alignment->coords[n1][1];
  new_alignment->coords[n1][0] = new_alignment->coords[n2][0];
  new_alignment->coords[n1][1] = new_alignment->coords[n2][1];
  new_alignment->coords[n2][0] = n10;
  new_alignment->coords[n2][1] = n11;
  generate_net(new_alignment, new_alignment->coords, new_alignment->num_coords);
  return new_alignment;
}

struct Alignment * move(struct Alignment * alignment, int node, int * coords) {
  struct Alignment * new_alignment = malloc(sizeof(struct Alignment));
  new_alignment->num_coords = alignment->num_coords;
  // allocate coords
  new_alignment->coords = malloc(alignment->num_coords * sizeof(int*));
  // copy coords
  for (size_t i = 0; i < alignment->num_coords; i++) {
    new_alignment->coords[i] = malloc(2 * sizeof(int));
    new_alignment->coords[i][0] = alignment->coords[i][0];
    new_alignment->coords[i][1] = alignment->coords[i][1];
  }
  // move coords
  new_alignment->coords[node][0] = coords[0];
  new_alignment->coords[node][1] = coords[1];
  generate_net(new_alignment, new_alignment->coords, new_alignment->num_coords);
  return new_alignment;
}

void get_neighbors(struct Alignment * alignment, struct Alignment ** neighbors, int * num_neighbors) {
  int i, j, k;
  *num_neighbors = 0;
  for (i = 0; i < alignment->num_coords; i++)
    for (j = 0; j < alignment->num_coords; j++)
      if (i != j)
        neighbors[(*num_neighbors)++] = swap(alignment, i, j);
  for (i = 0; i < alignment->num_coords; i++)
    for (j = 0; j < network.rows; j++)
      for (k = 0; k < network.cols; k++)
        if (alignment->net[j][k] == 0) {
          int * coords = malloc(2 * sizeof(int));
          coords[0] = j;
          coords[1] = k;
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
  return passed / (double)solution->num_coords;
}

struct Alignment * get_best_neighbor(struct Alignment * alignment, struct Alignment * solution) {
  struct Alignment ** neighbors = malloc(sizeof(struct Alignment *) * 10000);
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

int readfiles(char * filename, int ** coords, int * num_coords, int * rows, int * cols) {
  int i;
  char * line = NULL, * token;
  size_t len = 0;
  size_t read;
  int line_num = -1;
  FILE * handle = fopen(filename, "r");
  if (handle == NULL)
    exit(EXIT_FAILURE);
  *num_coords = 0;
  while ((read = getline(&line, &len, handle)) != -1) {
    token = strtok(line, " ");
    if (line_num == -1) {
      *rows = atoi(token);
      token = strtok(NULL, " ");
      *cols = atoi(token);
      line_num++;
      continue;
    }
    int token_num = 0;
    while (token != NULL) {
      int value = atoi(token);
      if (value == 1) {
        coords[*num_coords] = malloc(2 * sizeof(int));
        coords[*num_coords][0] = line_num;
        coords[(*num_coords)++][1] = token_num;
      }
      token = strtok(NULL, " ");
      token_num++;
    }
    line_num++;
  }
  fclose(handle);
  if (line)
      free(line);
}

int main(int argc, char * argv[]) {
  /*
   * Usage: ./mini <#nodes> <timeout> <input puzzle> <input start position>
   */
  int i;
  double t, es, es_new, p;
  bool accept;
  int ** sol_coords, ** s_coords;
  int rows, cols, num_sol_coords, num_s_coords;
  sol_coords = malloc(atoi(argv[1]) * sizeof(int*));
  s_coords = malloc(atoi(argv[1]) * sizeof(int*));
  struct Alignment * s = malloc(sizeof(struct Alignment)),
                   * sol = malloc(sizeof(struct Alignment)),
                   * s_new;
  readfiles(argv[3], sol_coords, &num_sol_coords, &rows, &cols);
  readfiles(argv[4], s_coords, &num_s_coords, &rows, &cols);
  init_network(rows, cols);
  generate_net(sol, sol_coords, num_sol_coords);
  generate_net(s, s_coords, num_s_coords);
  printf("STARTING STATE:\n");
  print_net(s->net, rows, cols);
  printf("\nGOAL STATE:\n");
  print_net(sol->net, rows, cols);
  printf("\npress enter> ");
  getchar();
  for (i = 0; i < atoi(argv[2]); i++) {
    if (!cur_term) {
      int result;
      setupterm( NULL, STDOUT_FILENO, &result );
    }
    putp( tigetstr( "clear" ) );
    s_new = get_best_neighbor(s, sol);
    t = temperature(i);
    printf("temp: %f, time=%d\n", t, i);
    print_net(s->net, rows, cols);
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
  print_net(s->net, rows, cols);
  free(s);
  free(sol);
  exit(EXIT_SUCCESS);
}
