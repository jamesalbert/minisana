#ifndef SCORE_H
#define SCORE_H

#include <stdbool.h>
#include <stdlib.h>
#include "structures.h"

void edge_coverage(MiniMan_t *, short int, bool);
void full_edge_coverage(MiniMan_t *);
void sequence_similarity(MiniMan_t *, short int, bool);
void full_sequence_similarity(MiniMan_t *);
void update_score(MiniMan_t *);
void subtract_score(MiniMan_t *, short int);
void add_score(MiniMan_t *, short int);

#endif
