#define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <term.h>


struct Network {
  int ** graph;
  int rows;
  int cols;
} network;

struct Alignment {
  struct Network net; // adjacency matrix representation of network
  int ** coords;      // coordinates of each node
  int num_coords;     // number of coordinates
};


void init_network(int rows, int cols) {
  network.rows = rows;
  network.cols = cols;
  network.graph = malloc(rows * sizeof(int*));
  for (int i = 0; i < rows; i++) {
    network.graph[i] = malloc(cols * sizeof(int));
    for (int j = 0; j < cols; j++)
      network.graph[i][j] = 0;
  }
}

void print_net(struct Network net) {
  for (int i = 0; i < net.rows; i++) {
    for (int j = 0; j < net.cols; j++)
      printf("%d ", net.graph[i][j]);
    printf("\n");
  }
}

void print_coords(int ** coords, int num_coords) {
  printf("{");
  for (int i = 0; i < num_coords; i++)
    printf("\n\t[ %d, %d ]", coords[i][0], coords[i][1]);
  printf("\n}\n");
}

void copy_network(struct Alignment * alignment, struct Network net) {
  size_t i, j;
  for (i = 0; i < net.rows; i++) {
    alignment->net.graph[i] = malloc(net.cols * sizeof(int));
    for (j = 0; j < net.cols; j++)
      alignment->net.graph[i][j] = 0;
  }
  alignment->net.rows = i;
  alignment->net.cols = j;
}

void plot_coords(struct Alignment * alignment, int ** coords, int num_coords) {
  for (size_t i = 0; i < num_coords; i++) {
    alignment->net.graph[coords[i][0]][coords[i][1]] = 1;
  }
  memcpy(&alignment->coords, &coords, sizeof(coords));
  alignment->num_coords = num_coords;
}

void create_network(struct Alignment * alignment, int ** coords, int num_coords) {
  alignment->net.graph = malloc(network.rows * sizeof(int*));
  copy_network(alignment, network);
  plot_coords(alignment, coords, num_coords);
}

void create_alignment(struct Alignment * to, struct Alignment * from) {
  to->num_coords = from->num_coords;
  to->coords = malloc(to->num_coords * sizeof(int*));
  for (size_t i = 0; i < to->num_coords; i++) {
    to->coords[i] = malloc(2 * sizeof(int));
    to->coords[i][0] = from->coords[i][0];
    to->coords[i][1] = from->coords[i][1];
  }
}

void destroy_alignment(struct Alignment * alignment) {
  for (size_t i = 0; i < alignment->num_coords; i++)
    free(alignment->coords[i]);
  free(alignment->coords);
  for (size_t i = 0; i < alignment->net.rows; i++)
    free(alignment->net.graph[i]);
  free(alignment->net.graph);
  free(alignment);
}

struct Alignment * swap(struct Alignment * alignment, int n1, int n2) {
  struct Alignment * neighbor = malloc(sizeof(struct Alignment));
  create_alignment(neighbor, alignment);
  int n10 = neighbor->coords[n1][0];
  int n11 = neighbor->coords[n1][1];
  neighbor->coords[n1][0] = neighbor->coords[n2][0];
  neighbor->coords[n1][1] = neighbor->coords[n2][1];
  neighbor->coords[n2][0] = n10;
  neighbor->coords[n2][1] = n11;
  create_network(neighbor, neighbor->coords, neighbor->num_coords);
  return neighbor;
}

struct Alignment * move(struct Alignment * alignment, int node, int c1, int c2) {
  struct Alignment * neighbor = malloc(sizeof(struct Alignment));
  create_alignment(neighbor, alignment);
  neighbor->coords[node][0] = c1;
  neighbor->coords[node][1] = c2;
  create_network(neighbor, neighbor->coords, neighbor->num_coords);
  return neighbor;
}

double edge_cover(struct Alignment * alignment, struct Alignment * solution) {
  double passed = 0.0;
  for (size_t i = 0; i < alignment->num_coords; i++)
    for (size_t j = 0; j < solution->num_coords; j++)
      if (alignment->coords[i][0] == solution->coords[j][0] &&
          alignment->coords[i][1] == solution->coords[j][1])
        passed++;
  return passed / (double)solution->num_coords;
}

struct Alignment * get_neighbors(struct Alignment * alignment, struct Alignment * solution) {
  int i, j, k;
  double best_score_yet = 0.0;
  struct Alignment * best_neighbor = malloc(sizeof(struct Alignment));
  for (i = 0; i < alignment->num_coords; i++)
    for (j = 0; j < alignment->num_coords; j++)
      if (i != j) {
        struct Alignment * neighbor = swap(alignment, i, j);
        double current_score = edge_cover(neighbor, solution);
        if (current_score > best_score_yet) {
          best_score_yet = current_score;
          best_neighbor = neighbor;
        } else {
          destroy_alignment(neighbor);
        }
      }
  for (i = 0; i < alignment->num_coords; i++)
    for (j = 0; j < network.rows; j++)
      for (k = 0; k < network.cols; k++)
        if (alignment->net.graph[j][k] == 0) {
          struct Alignment * neighbor = move(alignment, i, j, k);
          double current_score = edge_cover(neighbor, solution);
          if (current_score > best_score_yet) {
            best_score_yet = current_score;
            best_neighbor = neighbor;
          } else {
            destroy_alignment(neighbor);
          }
        }
  return best_neighbor;
}

struct Alignment * get_best_neighbor(struct Alignment * alignment, struct Alignment * solution) {
  struct Alignment * best_neighbor;
  best_neighbor = get_neighbors(alignment, solution);
  return best_neighbor;
}

double probability(double es, double es_new, double t) {
  return exp(-(es_new - es) / t);
}

double temperature(double k) {
  return 1 * exp(-1 * (k / 100.0));
}

int readfiles(char * filename, int ** coords, int * num_coords, int * rows, int * cols) {
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
  create_network(sol, sol_coords, num_sol_coords);
  create_network(s, s_coords, num_s_coords);
  printf("STARTING STATE:\n");
  print_net(s->net);
  printf("\nGOAL STATE:\n");
  print_net(sol->net);
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
    print_net(s->net);
    es = edge_cover(s, sol);
    es_new = edge_cover(s_new, sol);
    p = probability(es, es_new, t);
    if (es_new - es >= 0) {
      destroy_alignment(s);
      s = s_new;
    } else {
      accept = (rand() % 100) < 1 - p;
      if (accept) {
        destroy_alignment(s);
        s = s_new;
      }
      else
        destroy_alignment(s_new);
    }
  }
  printf("\nfound:\n");
  print_net(s->net);
  destroy_alignment(s);
  destroy_alignment(sol);
  exit(EXIT_SUCCESS);
}
