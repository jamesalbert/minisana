#define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <term.h>
#include <sys/stat.h>
#include <regex.h>
#include <trie.h>
#include <arraylist.h>
#include "structures.h"
#include "graph.h"
#include "topology.h"
#include "create.h"
#include "copy.h"
#include "destroy.h"

#define TIME 100000000

void move(struct Alignment * alignment, char * tail, struct Node * head) {
  trie_insert(alignment->map, tail, head);
}

void swap(struct Alignment * neighbor, char * tail, char * head) {
  struct Node * old_head = (struct Node *)trie_lookup(neighbor->map, tail),
              * new_head = (struct Node *)trie_lookup(neighbor->map, head);
  trie_insert(neighbor->map, tail, new_head);
  trie_insert(neighbor->map, head, old_head);
}

void get_rand_neighbor(struct Alignment * alignment, bool undo) {
  bool will_swap;
  int node1, node2;
  char * tail, * head;
  if (undo) {
    will_swap = alignment->last_move[0];
    node1 = alignment->last_move[2];
    node2 = alignment->last_move[1];
  } else {
    will_swap = rand() & 1;
    node1 = rand() % G1->num_nodes;
    node2 = rand() % G1->num_nodes;
  }
  tail = G1->nodes[node1]->name;
  head = will_swap ? G1->nodes[node2]->name : NULL;
  alignment->score -= small_edge_coverage(alignment, tail, head);
  if (will_swap)
    swap(alignment, tail, head);
  else {
    struct Node * node = G2->nodes[node2];
    move(alignment, tail, node);
  }
  alignment->score += small_edge_coverage(alignment, tail, head);
  alignment->score = fabs(alignment->score);
  alignment->last_move[0] = (int)will_swap;
  alignment->last_move[1] = node1;
  alignment->last_move[2] = node2;
}

double probability(double es, double es_new, double t) {
  return exp(-(es_new - es) / t);
}

double temperature(double k) {
  return 1 * exp(-1 * (k / (double)TIME));
}

int main(int argc, char * argv[]) {
  /*
   * Usage: ./mini <smaller network> <larger network>
   */
  struct Alignment * s = malloc(sizeof(struct Alignment));
  create_alignment(s, argv);
  double t, p, prev_score;
  bool accept;
  printf("\n");
  for (int i = 0; i < TIME; i++) {
    prev_score = s->score;
    get_rand_neighbor(s, false);
    t = temperature(i);
    if (i % 100 == 0) {
      printf("\033[A\r<%% Generated at temp: %f, time: %d, score: %f, edges aligned: %.0f / %d %%>\n",
      t, i, s->score, s->score * G1->num_edges, G1->num_edges);
    }
    p = probability(prev_score, s->score, t);
    if (s->score - prev_score < 0) {
      accept = (rand() % 100) < p;
      if (accept)
        get_rand_neighbor(s, true);
    }
  }
  printf("\n");
  destroy_graph(G1);
  destroy_graph(G2);
  // destroy_adj(A1);
  destroy_adj(A2);
  destroy_alignment_copy(s);
  exit(EXIT_SUCCESS);
}
