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

struct Alignment * get_best_neighbor(struct Alignment * alignment) {
  int i, j, k, processed = 0;
  double best_score_yet = 0.0, current_score;
  struct Alignment * best_neighbor = malloc(sizeof(struct Alignment));
  copy_alignment(alignment, best_neighbor);
  for (i = 0; i < alignment->map->smaller->num_nodes; i++) {
    for (j = 0; j < alignment->map->smaller->num_nodes; j++) {
      if (i != j) {
        // struct Node * tail = alignment->map->smaller->nodes[i],
        //             * head = alignment->map->smaller->nodes[j];
        char * tail = alignment->map->smaller->nodes[i]->name,
             * head = alignment->map->smaller->nodes[j]->name;
        swap(alignment, tail, head);
        current_score = edge_coverage_swap(alignment, tail, head);
        if (current_score > best_score_yet) {
          best_score_yet = current_score;
          destroy_alignment_copy(best_neighbor);
          best_neighbor = malloc(sizeof(struct Alignment));
          copy_alignment(alignment, best_neighbor);
          best_neighbor->score = current_score;
        }
        swap(alignment, head, tail);
      }
    }
  }
  // for (i = 0; i < alignment->map->smaller->num_nodes; i++) {
  //   for (j = 0; i < alignment->map->larger->num_nodes; j++) {
  //     char * tail = alignment->map->smaller->nodes[i]->name;
  //     struct Node * old_head = translate(alignment, tail),
  //                 * new_head = alignment->map->larger->nodes[j];
  //     if (old_head == new_head)
  //       continue;
  //     move(alignment, tail, new_head);
  //     current_score = edge_cover(alignment);
  //     if (current_score > best_score_yet) {
  //       best_score_yet = current_score;
  //       destroy_alignment_copy(best_neighbor);
  //       best_neighbor = malloc(sizeof(struct Alignment));
  //       copy_alignment(alignment, best_neighbor);
  //     }
  //   }
  // }
  return best_neighbor;
}


double probability(double es, double es_new, double t) {
  return exp(-(es_new - es) / t);
}

double temperature(double k) {
  return 1 * exp(-1 * (k / 100.0));
}

int main(int argc, char * argv[]) {
  /*
   * Usage: ./mini <#nodes> <timeout> <input puzzle> <input start position>
   */
  struct Alignment * s = malloc(sizeof(struct Alignment)),
                   * s_new;
  create_alignment(s, argv);
  double t, p;
  bool accept;
  for (int i = 0; i < 1; i++) {
    s_new = get_best_neighbor(s);
    t = temperature(i);
    printf("<%% Generated at temp: %f, time=%d, current_score=%f %%>\n", t, i, s_new->score);
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
  if (s != NULL)
    destroy_alignment(s);
  if (s_new != NULL)
    destroy_alignment_copy(s_new);
  exit(EXIT_SUCCESS);
}
