#define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <term.h>
#include <regex.h>
#include <trie.h>
#include <arraylist.h>

#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#define max(X,Y) ((X) > (Y) ? (X) : (Y))

#include "structures.h"
#include "graph.h"
#include "topology.h"
#include "create.h"
#include "copy.h"
#include "destroy.h"
#include "print.h"

void move(struct Alignment * alignment, char * tail, struct Node * head) {
  trie_insert(alignment->map->translation, tail, head);
}

void swap(struct Alignment * neighbor, char * tail, char * head) {
  struct Node * old_head = (struct Node *)trie_lookup(neighbor->map->translation, tail),
              * new_head = (struct Node *)trie_lookup(neighbor->map->translation, head);
  trie_insert(neighbor->map->translation, tail, new_head);
  trie_insert(neighbor->map->translation, head, old_head);
}

struct Alignment * get_rand_neighbor(struct Alignment * alignment, int node1, int node2, bool will_swap) {
  struct Alignment * neighbor = malloc(sizeof(struct Alignment));
  copy_alignment(alignment, neighbor);
  char * tail = neighbor->map->smaller->nodes[node1]->name,
       * head = neighbor->map->smaller->nodes[node2]->name;
  if (will_swap) {
    swap(neighbor, tail, head);
    neighbor->score = update_edge_coverage(neighbor, tail, head);
  } else {
    char * tail = neighbor->map->smaller->nodes[node1]->name;
    struct Node * node = alignment->map->larger->nodes[node2];
    move(neighbor, tail, node);
    neighbor->score = update_edge_coverage(neighbor, tail, NULL);
  }
  return neighbor;
}

double probability(double es, double es_new, double t) {
  return exp(-(es_new - es) / t);
}

double temperature(double k) {
  return 1 * exp(-1 * (k / 100000.0));
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
  for (int i = 0; i < 100000; i++) {
    will_swap = rand() & 1;
    random_node1 = rand() % s->map->smaller->num_nodes;
    random_node2 = rand() % s->map->smaller->num_nodes;
    s_new = get_rand_neighbor(s, random_node1, random_node2, will_swap);
    t = temperature(i);
    if (i % 100 == 0) {
      printf("\033[A\r<%% Generated at temp: %f, time: %d, score: %f, edges aligned: %.0f / %d %%>\n",
      t, i, s_new->score, s_new->score * s->map->smaller->num_edges, s->map->smaller->num_edges);
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
  if (s != NULL)
    destroy_alignment_copy(s);
  exit(EXIT_SUCCESS);
}
