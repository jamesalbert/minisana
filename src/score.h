#ifndef SCORE_H
#define SCORE_H

#include <stdbool.h>
#include <stdlib.h>
#include "create.h"

void edge_coverage(short int, bool);
void full_edge_coverage();
void sequence_similarity(short int, bool);
void full_sequence_similarity();
void update_score();
void subtract_score(short int);
void add_score(short int);

#endif
