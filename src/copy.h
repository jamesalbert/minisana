#ifndef COPY_H
#define COPY_H

void copy_mapping(struct Alignment * origalign, Trie * copy) {
  Trie * orig = origalign->map;
  if (copy == NULL) {
    perror("trie_new error");
    exit(EXIT_FAILURE);
  }
  int err;
  for (size_t i = 0; i < trie_num_entries(orig); i++) {
    err = trie_insert(copy,
                      G1->nodes[i]->name,
                      translate(origalign, G1->nodes[i]->name));
    if (err == 0) {
      perror("trie_insert error");
      exit(EXIT_FAILURE);
    }
  }
}

void copy_alignment(struct Alignment * orig, struct Alignment * copy) {
  copy->map = trie_new();
  copy->score = orig->score;
  copy_mapping(orig, copy->map);
}

#endif
