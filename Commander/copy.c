#include <sys/sendfile.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libgen.h>
#include "copy.h"

#define MAX_REALPATH 4096

int copy_regular_file(int sourcefd, int destfd, int size) {
  size_t result;
  result = sendfile(destfd, sourcefd, 0, size);
  // TODO:  "sendfile() will transfer at most 0x7ffff000 (2,147,479,552)
  //        bytes"
  if (result != size) return -1;
  return 0;
}

mode_t copy_get_mode(char *path) {
  struct stat stat_buff;
  stat(path, &stat_buff);
  return stat_buff.st_mode;
}

bool copy_is_directory(char *path) {
  return S_ISDIR(copy_get_mode(path));
}

bool file_exists(char *path) {
  struct stat stat_buffer;
  return (stat(path, &stat_buffer) == 0);
}

size_t copy_get_size(char *path) {
  struct stat stat_buff;
  stat(path, &stat_buff);
  return stat_buff.st_size;
}

int copy_regular_file_names(char *source_name, char *dest_name, bool verbose) {
  if (verbose) printf("Copying %s to %s\n", source_name, dest_name);
  int dest_fd = open(dest_name, O_CREAT | O_WRONLY | O_TRUNC, copy_get_mode(source_name));
  int source_fd = open(source_name, O_RDONLY);
  int result = copy_regular_file(source_fd, dest_fd, copy_get_size(source_name));
  close(dest_fd);
  close(source_fd);
  if (result != 0) {
    fprintf(stderr, "Couldn't copy %s to %s\n", source_name, dest_name);
  }
  return result;
}

int copy_rec(char *path, char *dest, bool verbose) {
  char slash = '/';
  DIR *dir;
  struct dirent *ent;
  char *NullPositionPath = &path[strlen(path)];
  char *NullPositionDest = &dest[strlen(dest)];
  dir = opendir(path);
  if (dir == NULL) {
    fprintf(stderr, "Couldn't open %s directory\n", path);
    exit(EXIT_FAILURE);
  }
  mkdir(dest, copy_get_mode(path));
  while ((ent = readdir(dir)) != NULL) {
    if (ent->d_type == DT_REG) {
      char dest_name[MAX_REALPATH];
      char source_name[MAX_REALPATH];
      sprintf(source_name, "%s%c%s", path, slash, ent->d_name);
      sprintf(dest_name, "%s%c%s", dest, slash, ent->d_name);
      copy_regular_file_names(source_name, dest_name, verbose);
    }
    if (ent->d_type == DT_DIR) {
      if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0)) {
        sprintf(NullPositionPath, "%c%s", slash, ent->d_name);
        sprintf(NullPositionDest, "%c%s", slash, ent->d_name);
        if (copy_rec(path, dest, verbose) != 0) {
          closedir(dir);
          return -1;
        }
        *NullPositionPath = '\0';
        *NullPositionDest = '\0';
      }
    }
  }
  closedir(dir);
  return 0;
}

int copy(char *path, char *dest, bool rec, bool verbose) {
  if (rec != true && copy_is_directory(path)) {
    fprintf(stderr, "%s is a directory. Add -r to copy recursively\n", path);
    return -1;
  }
  if (copy_is_directory(path) && !copy_is_directory(dest) && file_exists(dest)) {
    fprintf(stderr, "Cannot overwrite non-directory %s with directory %s", dest, path);
    return -1;
  }
  if (!copy_is_directory(path) && !copy_is_directory(dest)) {
    return copy_regular_file_names(path, dest, verbose);
  }
  char pathmax_dest[MAX_REALPATH];
  strcpy(pathmax_dest, dest);
  if (!copy_is_directory(path) && copy_is_directory(dest)) {
    sprintf(&pathmax_dest[strlen(pathmax_dest)], "/%s", basename(path));
    return copy_regular_file_names(path, pathmax_dest, verbose);
  }
  if (copy_is_directory(path) && file_exists(dest)) {
    sprintf(&pathmax_dest[strlen(pathmax_dest)], "/%s", basename(path));
  }
  char pathmax_path[MAX_REALPATH];
  strcpy(pathmax_path, path);
  return copy_rec(pathmax_path, pathmax_dest, verbose);
}
