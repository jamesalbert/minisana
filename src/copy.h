#ifndef COPY_H
#define COPY_H

void copy_adj(struct Adjacency * orig, struct Adjacency * copy) {
  copy->dim = orig->dim;
  copy->matrix = orig->matrix;
}

void copy_mapping(struct Mapping * orig, struct Mapping * copy) {
  struct Graph * g1, * g2;
  copy->smaller = orig->smaller;
  copy->larger = orig->larger;
  copy->num_mappings = orig->num_mappings;
  copy->tails = malloc(orig->num_mappings * sizeof(struct Node *));
  copy->heads = malloc(orig->num_mappings * sizeof(struct Node *));
  for (size_t i = 0; i < copy->num_mappings; i++) {
    copy->tails[i] = orig->tails[i];
    copy->heads[i] = orig->heads[i];
  }
}

void copy_alignment(struct Alignment * orig, struct Alignment * copy) {
  struct Mapping * map = malloc(sizeof(struct Mapping));
  struct Adjacency * a1 = malloc(sizeof(struct Adjacency)),
                   * a2 = malloc(sizeof(struct Adjacency));
  copy_mapping(orig->map, map);
  copy_adj(orig->a1, a1);
  copy_adj(orig->a2, a2);
  copy->map = map;
  copy->a1 = a1;
  copy->a2 = a2;
}

#endif
