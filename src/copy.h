#ifndef COPY_H
#define COPY_H

void copy_mapping(struct Alignment * origalign, struct Mapping * copy) {
  struct Mapping * orig = origalign->map;
  copy->smaller = orig->smaller;
  copy->larger = orig->larger;
  copy->num_mappings = orig->num_mappings;
  copy->translation = trie_new();
  if (copy->translation == NULL) {
    perror("trie_new error");
    exit(EXIT_FAILURE);
  }
  int err;
  for (size_t i = 0; i < orig->num_mappings; i++) {
    err = trie_insert(copy->translation,
                orig->smaller->nodes[i]->name,
                translate(origalign, orig->smaller->nodes[i]->name));
    if (err == 0) {
      perror("trie_insert error");
      exit(EXIT_FAILURE);
    }
  }
}

void copy_alignment(struct Alignment * orig, struct Alignment * copy) {
  copy->map = malloc(sizeof(struct Mapping));
  copy->score = orig->score;
  copy_mapping(orig, copy->map);
  copy->a1 = orig->a1;
  copy->a2 = orig->a2;
}

#endif
