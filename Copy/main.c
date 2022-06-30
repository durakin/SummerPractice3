#include <stdio.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#include "copy.h"

int main(int argc, char *argv[]) {
  char* filename1 = argv[1];
  char* filename2 = argv[2];

  copy(filename1, filename2, true);
  return 0;
}
