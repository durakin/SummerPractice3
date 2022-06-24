#include <stdio.h>
#include <stdlib.h>
#include "calc.h"
#include <sys/resource.h>
#include <stdint-gcc.h>
#include <string.h>

unsigned long int stack_limit() {
  struct rlimit limit;
  getrlimit(RLIMIT_STACK, &limit);
  return limit.rlim_cur;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "No expression");
    exit(EXIT_FAILURE);
  }
  if (argc > 2) {
    fprintf(stderr, "Too many arguments");
    exit(EXIT_SUCCESS);
  }

  uint32_t arg_size = strlen(argv[1]);

  if (arg_size >= stack_limit()) {
    fprintf(stderr, "Expression is too long");
    exit(EXIT_FAILURE);
  }

  char posfixbuff[arg_size];
  posfixbuff[0] = '\0';
  to_postfix_notation(argv[1], posfixbuff);
  fprintf(stdout, "%f\n", calculate_postfix_notation(posfixbuff));
  exit(EXIT_SUCCESS);
}
