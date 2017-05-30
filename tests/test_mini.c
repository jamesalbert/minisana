#include <criterion/criterion.h>
#include "mini.h"
#include "structures.h"
#include "create.h"

MiniMan_t * mm;

void setup(void) {
  mm = new_miniman();
  mm->g1_file = "input/yeast.gw";
  mm->g2_file = "input/human.gw";
  mm->seq_file = "input/yeast_human.bitscores";
  mm->output_file = "output/mini.out.json";
  mm->alpha = 0.5;
  mm->time = 10000;
  create_alignment(mm);
}

void teardown(void) {
  free(mm);
}

Test(mini, test_mini_probability, .init = setup, .fini = teardown) {
  double p = probability(mm, 0.006408, 0.83);
  printf("%f\n", p);
}
