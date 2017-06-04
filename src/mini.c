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

void move(MiniMan_t * mm, int node1, int node2, int old) {
  if (mm->G2->taken[node2] == 1)
    return;
  mm->G1->translate[node1] = node2;
  mm->G2->taken[node2] = 1;
  mm->G2->taken[old] = 0;
}

void swap(MiniMan_t * mm, int node1, int node2) {
  if (node1 == node2)
    return;
  int temp = mm->G1->translate[node1];
  mm->G1->translate[node1] = mm->G1->translate[node2];
  mm->G1->translate[node2] = temp;
}

void get_rand_neighbor(MiniMan_t * mm, bool undo) {
  bool will_swap;
  short int node1, node2, old;
  if (undo) {
    will_swap = mm->A->last_move[0];
    node1 = (will_swap ? mm->A->last_move[2] : mm->A->last_move[1]);
    node2 = (will_swap ? mm->A->last_move[1] : mm->A->last_move[3]);
  } else {
    will_swap = rand() & 1;
    node1 = rand() % mm->G1->num_nodes;
    node2 = rand() % (will_swap ? mm->G1->num_nodes : mm->G2->num_nodes);
  }
  old = mm->G1->translate[node1];
  subtract_score(mm, node1);
  if (will_swap) {
    subtract_score(mm, node2);
    swap(mm, node1, node2);
    add_score(mm, node2);
  } else
    move(mm, node1, node2, old);
  add_score(mm, node1);
  update_score(mm);
  mm->A->last_move[0] = will_swap;
  mm->A->last_move[1] = node1;
  mm->A->last_move[2] = node2;
  mm->A->last_move[3] = old;
}

double probability(MiniMan_t * mm, double prev_score, double t) {
  return exp(-(mm->score - prev_score) / t);
}

double temperature(MiniMan_t * mm, double i) {
  return T_INITIAL * exp(-T_DECAY * (i / (double)mm->time));
}

int main(int argc, char * argv[]) {
  /*
   * Usage: ./mini <smaller network> <larger network>
  */
  MiniMan_t * mm;
  mm = new_miniman();
  parse_args(argc, argv, mm);
  create_alignment(mm);
  double t, p, prev_score;
  printf("\n");
  for (int i = 0; i < mm->time; i++) {
    prev_score = mm->score;
    get_rand_neighbor(mm, false);
    t = temperature(mm, i);
    if (i % INTERVAL == 0)
      print_status(mm, t, i);
    p = probability(mm, prev_score, t);
    if (mm->score - prev_score < 0)
      if ((rand() % 100) < p)
        get_rand_neighbor(mm, true);
  }
  printf("\n");
  print_mapping(mm);
  free_everything(mm);
  exit(EXIT_SUCCESS);
}
