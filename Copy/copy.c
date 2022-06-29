#include "copy.h"
#include <sys/sendfile.h>
#include <sys/stat.h>

int copy(int sourcefd, int destfd) {
  struct stat stat_buf;
  fstat(sourcefd, &stat_buf);
  ssize_t size;
  size = stat_buf.st_size;
  ssize_t result;
  result = sendfile(destfd, sourcefd, 0, stat_buf.st_size);
  // TODO:  "sendfile() will transfer at most 0x7ffff000 (2,147,479,552)
  //        bytes"
  if (result != size) return -1;
  return 0;
}