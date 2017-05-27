#include "opts.h"

int parse_args(int argc, char * argv[], char * files[], double * alpha) {
  files[0] = NULL;
  files[1] = NULL;
  files[2] = NULL;
  *alpha = 0.0;
  char c;
  while ((c = getopt (argc, argv, "n:N:s:a:")) != -1)
    switch (c) {
      case 'n':
        files[0] = optarg;
        break;
      case 'N':
        files[1] = optarg;
        break;
      case 's':
        files[2] = optarg;
        break;
      case 'a':
        *alpha = atof(optarg);
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
  if (files[0] == NULL || files[1] == NULL) {
    fprintf(stderr, "Options -n,-N are required.\n");
    exit(EXIT_FAILURE);
  }
  // define optional opts
  if (files[2] == NULL)
    files[2] = "input/yeast_human.bitscores";
  if (*alpha == 0.0)
    *alpha = 0.5;
}
