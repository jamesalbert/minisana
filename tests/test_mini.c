#include <criterion/criterion.h>
#include "mini.h"
#include "structures.h"
#include "create.h"

Test(mini, test_mini_probability) {
  printf("test\n");
  mm = malloc(sizeof(struct MiniMan));
  mm->g1_file = "../input/syeast0.gw";
  mm->g2_file = "../input/yeast.gw";
  mm->seq_file = "../input/yeast_human.bitscores";
  mm->output_file = "../output/mini.out.json";
  mm->alpha = 0.5;
  mm->time = 10;
  A = malloc(sizeof(struct Alignment));
  create_alignment(mm);
}
