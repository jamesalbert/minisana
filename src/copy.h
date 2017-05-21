#ifndef COPY_H
#define COPY_H

void copy_adj(struct Adjacency * orig, struct Adjacency * copy) {
  copy->dim = orig->dim;
  copy->matrix = orig->matrix;
}

void copy_mapping(struct Mapping * orig, struct Mapping * copy) {
  copy->smaller = orig->smaller;
  copy->larger = orig->larger;
  copy->num_mappings = orig->num_mappings;
  // copy->tails = malloc(orig->num_mappings * sizeof(struct Node *));
  // copy->heads = malloc(orig->num_mappings * sizeof(struct Node *));
  copy->translation = trie_new();
  if (copy->translation == NULL) {
    perror("trie_new error");
    exit(EXIT_FAILURE);
  }
  int err;
  for (size_t i = 0; i < orig->num_mappings; i++) {
    err = trie_insert(copy->translation,
                orig->smaller->nodes[i]->name,
                orig->larger->nodes[i]);
    if (err == 0) {
      perror("trie_insert error");
      exit(EXIT_FAILURE);
    }
    // copy->tails[i] = orig->tails[i];
    // copy->heads[i] = orig->heads[i];

  }
}

void copy_alignment(struct Alignment * orig, struct Alignment * copy) {
  copy->map = malloc(sizeof(struct Mapping));
  copy->a1 = malloc(sizeof(struct Adjacency)),
  copy->a2 = malloc(sizeof(struct Adjacency));
  copy_mapping(orig->map, copy->map);
  copy_adj(orig->a1, copy->a1);
  copy_adj(orig->a2, copy->a2);
}

#endif
