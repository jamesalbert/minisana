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
  double p, t = 0.5;
  for (double i = 0.0; i < mm->A->score; i += 0.0001) {
    double p = probability(mm, i, t),
           e = exp(-(mm->A->score - i) / t);
    cr_assert(0 <= p && p <= 1, "probability outside range [0, 1]");
    cr_assert(0 <= e && e <= 1, "probability outside range [0, 1]");
    cr_assert(p == e, "unexpected probability");
  }
}
