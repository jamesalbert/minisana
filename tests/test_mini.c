#include <criterion/criterion.h>
#include "mini.h"
#include "structures.h"
#include "create.h"
#include "read.h"
#include "print.h"
#include "opts.h"

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
  double i, p, t = 0.5;
  for (i = 0.0; i < mm->score; i += 0.0001) {
    double p = probability(mm, i, t),
           e = exp(-(mm->score - i) / t);
    cr_assert(0 <= p && p <= 1, "probability outside range [0, 1]");
    cr_assert(0 <= e && e <= 1, "probability outside range [0, 1]");
    cr_assert(p == e, "unexpected probability");
  }
}

Test(mini, test_mini_temperature, .init = setup, .fini = teardown) {
  double t = temperature(mm, 0.0);
  int i;
  cr_assert(t = 1.0, "expected 1.0 temperature as base case");
  for (i = 1; i < 100; ++i) {
    t = temperature(mm, i);
    double e = exp(-T_DECAY * (i / (double)mm->time));
    cr_assert(t == e, "unexpected temperature");
  }
}

Test(mini, test_mini_get_rand_neighbor, .init = setup, .fini = teardown) {
  // short int old = mm->G1->translate[j];
  get_rand_neighbor(mm, false);
  int stored_will_swap = mm->A->last_move[0];
  // int stored_old = mm->A->last_move[3];
  cr_assert(0 <=  stored_will_swap && stored_will_swap <= 1, "expected will_swap to be in range [0, 1]");
  // cr_assert(old == stored_old, "expected old to be stored in last_move[3]");
}

Test(mini, test_mini_get_rand_neighbor_undo, .init = setup, .fini = teardown) {
  // short int old = mm->G1->translate[j];
  get_rand_neighbor(mm, false);
  get_rand_neighbor(mm, true);
  int stored_will_swap = mm->A->last_move[0];
  // int stored_old = mm->A->last_move[3];
  cr_assert(0 <=  stored_will_swap && stored_will_swap <= 1, "expected will_swap to be in range [0, 1]");
  // cr_assert(old == stored_old, "expected old to be stored in last_move[3]");
}

Test(mini, test_mini_move, .init = setup, .fini = teardown) {
  move(mm, 0, mm->G2->num_nodes - 1, 0);
}

Test(mini, test_mini_move_taken, .init = setup, .fini = teardown) {
  move(mm, 0, 1, 0);
}

Test(mini, test_read_missing_file, .init = setup, .fini = teardown) {
  short int ret = read_file("dont.exist", NULL, 0, NULL);
  cr_assert(ret == 1, "expected missing file to fail");
}

Test(mini, test_print_mapping, .init = setup, .fini = teardown) {
  print_mapping(mm);
}

Test(mini, test_print_status, .init = setup, .fini = teardown) {
  print_status(mm, 0.666, 1000);
}

Test(mini, test_print_node, .init = setup, .fini = teardown) {
  print_node(mm, 1, 0);
}

Test(mini, test_print_sequences, .init = setup, .fini = teardown) {
  print_sequences(mm);
}

Test(mini, test_opts, .init = setup, .fini = teardown) {
  char * args[] = {"./bin/mini", "-n", "input/yeast.gw", "-N", "input/human.gw",
                   "-t", "1000000", "-s", "input/yeast_human.bitscores",
                   "-a", "0.5", "-o", "output/mini.out.json"};
  parse_args(13, args, mm);
}
