#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "create.h"
#include "destroy.h"
#include "opts.h"
#include "print.h"
#include "score.h"
#include "structures.h"

#define T_INITIAL 1
#define T_DECAY 1000
#define INTERVAL 1000000

void intHandler(int);
void move(MiniMan_t *, int, int, int);
void swap(MiniMan_t *, int, int);
void get_rand_neighbor(MiniMan_t *, bool);
double probability(MiniMan_t *, double, double);
double temperature(MiniMan_t *, double);
int main(int argc, char * argv[]);
