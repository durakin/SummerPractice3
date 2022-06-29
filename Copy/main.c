#include <stdio.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#include "copy.h"

int main(int argc, char *argv[]) {
  char* filename1 = argv[1];
  char* filename2 = argv[2];

  int source = open(filename1, O_RDONLY);

  struct stat source_stat;
  fstat(source, &source_stat);
  int size;
  size = source_stat.st_size;
  mode_t mode;
  mode = source_stat.st_mode;

  int dest = open(filename2, O_CREAT | O_WRONLY | O_TRUNC, mode);

  close(source);
  close(dest);

  printf("%d", copy(source, dest, size));

  return 0;
}
