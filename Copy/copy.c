#include "copy.h"
#include <sys/sendfile.h>

int copy(int sourcefd, int destfd, int size) {
  size_t result;
  result = sendfile(destfd, sourcefd, 0, size);
  // TODO:  "sendfile() will transfer at most 0x7ffff000 (2,147,479,552)
  //        bytes"
  if (result != size) return -1;
  return 0;
}

