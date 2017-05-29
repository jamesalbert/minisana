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
void move(int, int, int);
void swap(int, int);
void get_rand_neighbor(bool);
double probability(double, double);
double temperature(double);
int main(int argc, char * argv[]);