#include "opts.h"

int parse_args(int argc, char * argv[], struct MiniMan * mm) {
  mm->g1_file = NULL;
  mm->g2_file = NULL;
  mm->seq_file = "input/yeast_human.bitscores";
  mm->output_file = "output/mini.out.json";
  mm->alpha = 0.5;
  mm->time = 100000000;
  char c;
  while ((c = getopt (argc, argv, "n:N:s:a:o:t:")) != -1)
    switch (c) {
      case 'n':
        mm->g1_file = optarg;
        break;
      case 'N':
        mm->g2_file = optarg;
        break;
      case 's':
        mm->seq_file = optarg;
        break;
      case 'o':
        mm->output_file = optarg;
        break;
      case 'a':
        mm->alpha = atof(optarg);
        break;
      case 't':
        mm->time = atoi(optarg);
        break;
      case '?':
        if (optopt == 'n' || optopt == 'N')
          fprintf(stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf(stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf(stderr,
                  "Unknown option character `\\x%x'.\n",
                  optopt);
        exit(EXIT_FAILURE);
      default:
        abort ();
      }
  // check required opts
  if (mm->g1_file == NULL || mm->g2_file == NULL) {
    fprintf(stderr, "Options -n,-N are required.\n");
    exit(EXIT_FAILURE);
  }
}
