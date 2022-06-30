#include "copy.h"
#include <sys/sendfile.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

char* find_first_slash(char *string) {
  int i = 0;
  while (i < strlen(string) && string[i] != '/') i++;
  return string + i;
}

int copy_regular_file(int sourcefd, int destfd, int size) {
  size_t result;
  result = sendfile(destfd, sourcefd, 0, size);
  // TODO:  "sendfile() will transfer at most 0x7ffff000 (2,147,479,552)
  //        bytes"
  if (result != size) return -1;
  return 0;
}

int copy(char *source_name, char *dest_name, bool rec) {
  int source = open(source_name, O_RDONLY);
  struct stat source_stat;
  fstat(source, &source_stat);
  int size;
  size = source_stat.st_size;
  mode_t mode;
  mode = source_stat.st_mode;

  if S_ISREG(mode) {
    int dest = open(dest_name, O_CREAT | O_WRONLY | O_TRUNC, mode);
    int result = copy_regular_file(source, dest, size);
    close(source);
    close(dest);
    return result;
  }

  if S_ISDIR(mode) {
    mkdir(dest_name, mode);
    close(source);
    DIR *dir;
    struct dirent *entry;

    dir = opendir(source_name);
    if (!dir) {
      exit(1);
    }

    while ((entry = readdir(dir)) != NULL) {
      char new_dest_name[1000];
      strcpy(new_dest_name, dest_name);
      strcat(new_dest_name, "/");
      strcat(new_dest_name, find_first_slash(source_name)+1);
      strcat(new_dest_name, "/");
      strcat(new_dest_name, entry->d_name);

      char new_source_name[1000];
      strcpy(new_source_name, source_name);
      strcat(new_source_name,"/");
      strcat(new_source_name, entry->d_name);

      copy(new_source_name, new_dest_name, true);
    };
    closedir(dir);
  }
  return 0;
}
