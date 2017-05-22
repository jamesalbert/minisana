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


void move(struct Alignment * alignment, char * tail, struct Node * head) {
  trie_insert(alignment->map, tail, head);
}

void swap(struct Alignment * neighbor, char * tail, char * head) {
  struct Node * old_head = (struct Node *)trie_lookup(neighbor->map, tail),
              * new_head = (struct Node *)trie_lookup(neighbor->map, head);
  trie_insert(neighbor->map, tail, new_head);
  trie_insert(neighbor->map, head, old_head);
}

struct Alignment * get_rand_neighbor(struct Alignment * alignment, int node1, int node2, bool will_swap) {
  struct Alignment * neighbor = malloc(sizeof(struct Alignment));
  copy_alignment(alignment, neighbor);
  char * tail = G1->nodes[node1]->name,
       * head = G1->nodes[node2]->name;
  if (will_swap) {
    swap(neighbor, tail, head);
    neighbor->score = update_edge_coverage(neighbor, tail, head);
  } else {
    char * tail = G1->nodes[node1]->name;
    struct Node * node = G2->nodes[node2];
    move(neighbor, tail, node);
    neighbor->score = update_edge_coverage(neighbor, tail, NULL);
  }
  return neighbor;
}

double probability(double es, double es_new, double t) {
  return exp(-(es_new - es) / t);
}

double temperature(double k) {
  return 1 * exp(-1 * (k / 1000.0));
}

int main(int argc, char * argv[]) {
  /*
   * Usage: ./mini <smaller network> <larger network>
   */
  struct Alignment * s = malloc(sizeof(struct Alignment)),
                   * s_new;
  create_alignment(s, argv);
  double t, p;
  bool accept, will_swap;
  int random_node1, random_node2;
  printf("\n");
  for (int i = 0; i < 100; i++) {
    will_swap = rand() & 1;
    random_node1 = rand() % G1->num_nodes;
    random_node2 = rand() % G1->num_nodes;
    s_new = get_rand_neighbor(s, random_node1, random_node2, will_swap);
    t = temperature(i);
    if (i % 100 == 0) {
      printf("\033[A\r<%% Generated at temp: %f, time: %d, score: %f, edges aligned: %.0f / %d %%>\n",
      t, i, s_new->score, s_new->score * G1->num_edges, G1->num_edges);
    }
    p = probability(s->score, s_new->score, t);
    if (s_new->score - s->score >= 0) {
      destroy_alignment_copy(s);
      s = s_new;
    } else {
      accept = (rand() % 100) < 1 - p;
      if (accept) {
        destroy_alignment_copy(s);
        s = s_new;
      }
      else
        destroy_alignment_copy(s_new);
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
