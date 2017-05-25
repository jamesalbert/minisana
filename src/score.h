#ifndef SCORE_H
#define SCORE_H

#include <stdlib.h>
#include "create.h"

unsigned int edge_coverage(short int);
unsigned int full_edge_coverage();
unsigned int sequence_similarity(short int);
unsigned int full_sequence_similarity();
void update_score();
void subtract_score(short int);
void add_score(short int);

#endif
