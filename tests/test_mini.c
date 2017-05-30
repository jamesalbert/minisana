#include <criterion/criterion.h>
#include "mini.h"
#include "structures.h"
#include "create.h"

struct MiniMan * mm;
struct Alignment * A;

void setup(void) {
  mm = malloc(sizeof(struct MiniMan));
  A = malloc(sizeof(struct Alignment));
  mm->g1_file = "input/yeast.gw";
  mm->g2_file = "input/human.gw";
  mm->seq_file = "input/yeast_human.bitscores";
  mm->output_file = "output/mini.out.json";
  mm->alpha = 0.5;
  mm->time = 10;
  create_alignment(A, mm);
}

void teardown(void) {
  free(mm);
  free(A);
}

Test(mini, test_mini_probability, .init = setup, .fini = teardown) {
  double p = probability(A, 0.01, 99.9);
  printf("%f\n", p);
}
