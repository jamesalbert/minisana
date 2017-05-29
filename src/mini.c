#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "create.h"
#include "destroy.h"
#include "opts.h"
#include "print.h"
#include "score.h"
#include "structures.h"

#define T_INITIAL 1
#define T_DECAY 1000
#define INTERVAL 1000000

void intHandler(int dummy) {
  print_mapping();
  free_everything();
  exit(EXIT_FAILURE);
}

void move(int node1, int node2, int old) {
  if (G2->taken[node2] == 1)
    return;
  G1->translate[node1] = node2;
  G2->taken[node2] = 1;
  G2->taken[old] = 0;
}

void swap(int node1, int node2) {
  int temp = G1->translate[node1];
  G1->translate[node1] = G1->translate[node2];
  G1->translate[node2] = temp;
}

void get_rand_neighbor(bool undo) {
  bool will_swap;
  short int node1, node2, old;
  if (undo) {
    will_swap = A->last_move[0];
    node1 = (will_swap ? A->last_move[2] : A->last_move[1]);
    node2 = (will_swap ? A->last_move[1] : A->last_move[3]);
  } else {
    will_swap = rand() & 1;
    node1 = rand() % G1->num_nodes;
    node2 = rand() % (will_swap ? G1->num_nodes : G2->num_nodes);
  }
  old = G1->translate[node1];
  subtract_score(node1);
  if (will_swap) {
    subtract_score(node2);
    swap(node1, node2);
    add_score(node2);
  } else
    move(node1, node2, old);
  add_score(node1);
  update_score();
  A->last_move[0] = will_swap;
  A->last_move[1] = node1;
  A->last_move[2] = node2;
  A->last_move[3] = old;
}

double probability(double prev_score, double t) {
  return exp(-(A->score - prev_score) / t);
}

double temperature(double k) {
  return T_INITIAL * exp(-T_DECAY * (k / (double)mm->time));
}

int main(int argc, char * argv[]) {
  /*
   * Usage: ./mini <smaller network> <larger network>
  */
  signal(SIGINT, intHandler);
  mm = malloc(sizeof(struct MiniMan));
  parse_args(argc, argv, mm);
  A = malloc(sizeof(struct Alignment));
  create_alignment(mm);
  double t, p, prev_score;
  printf("\n");
  for (int i = 0; i < mm->time; i++) {
    prev_score = A->score;
    get_rand_neighbor(false);
    t = temperature(i);
    if (i % INTERVAL == 0)
      print_status(t, i);
    p = probability(prev_score, t);
    if (A->score - prev_score < 0)
      if ((rand() % 100) < p)
        get_rand_neighbor(true);
  }
  printf("\n");
  print_mapping();
  free_everything();
  exit(EXIT_SUCCESS);
}
