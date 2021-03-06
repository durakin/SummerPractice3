#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <ftw.h>
#include <unistd.h>
#include "rm.h"

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
  int rv = remove(fpath);

  if (rv)
    perror(fpath);

  return rv;
}

int rm(char *path)
{
  return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}