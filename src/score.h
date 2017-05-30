#ifndef SCORE_H
#define SCORE_H

#include <stdbool.h>
#include <stdlib.h>
#include "structures.h"

void edge_coverage(struct Alignment *, short int, bool);
void full_edge_coverage(struct Alignment *);
void sequence_similarity(struct Alignment *, short int, bool);
void full_sequence_similarity(struct Alignment *);
void update_score(struct Alignment *, struct MiniMan *);
void subtract_score(struct Alignment *, short int);
void add_score(struct Alignment *, short int);

#endif
