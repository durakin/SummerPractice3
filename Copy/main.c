#include "copy.h"

int main(int argc, char *argv[]) {
  char* filename1 = argv[1];
  char* filename2 = argv[2];

  return copy(filename1, filename2, true, true);
}
