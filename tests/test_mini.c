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
  free_everything(mm);
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

Test(mini, test_mini_temperature, .init = setup, .fini = teardown) {
  double t = temperature(mm, 0.0);
  cr_assert(t = 1.0, "expected 1.0 temperature as base case");
  for (int i = 1; i < 100; ++i) {
    t = temperature(mm, i);
    double e = exp(-T_DECAY * (i / (double)mm->time));
    cr_assert(t == e, "unexpected temperature");
  }
}

Test(mini, test_mini_get_rand_neighbor, .init = setup, .fini = teardown) {
  get_rand_neighbor(mm, false);
}

Test(mini, test_mini_move, .init = setup, .fini = teardown) {
  move(mm, 0, 1, 0);
}
