#include <getopt.h>
#include "copy.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  char *filename1;
  char *filename2;

  bool recursive = 0;
  bool verbose = 0;

  opterr = 0;
  int op;
  while ((op = getopt(argc, argv, "rv")) != -1)
    switch (op) {
      case 'r':recursive = true;
        break;
      case 'v':verbose = true;
        break;
      default:printf("Unknown argument: %c", op);
        break;
    }

  printf("%d", argc);
  printf("%d", optind);
  if (argc - optind != 2) {
    fprintf(stderr, "Two positional arguments required");
    exit(EXIT_FAILURE);
  }
  filename1 = argv[optind];
  filename2 = argv[optind + 1];
  return copy(filename1, filename2, recursive, verbose);
}
