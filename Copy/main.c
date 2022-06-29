#include <stdio.h>
#include <sys/file.h>
#include "copy.h"

int main(int argc, char *argv[]) {
  char* filename1 = argv[1];
  char* filename2 = argv[2];

  int source = open(filename1, O_RDONLY);
  int dest = open(filename2, O_CREAT | O_WRONLY | O_TRUNC);
  printf("%d", copy(source, dest));

  return 0;
}
