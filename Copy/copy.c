#include "copy.h"
#include <sys/sendfile.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int copy_regular_file(int sourcefd, int destfd, int size) {
  size_t result;
  result = sendfile(destfd, sourcefd, 0, size);
  // TODO:  "sendfile() will transfer at most 0x7ffff000 (2,147,479,552)
  //        bytes"
  if (result != size) return -1;
  return 0;
}

mode_t get_mode(char *path) {
  struct stat stat_buff;
  stat(path, &stat_buff);
  return stat_buff.st_mode;
}

size_t get_size(char *path) {
  struct stat stat_buff;
  stat(path, &stat_buff);
  return stat_buff.st_size;
}

int copy_regular_file_names(char *source_name, char *dest_name) {
  int dest_fd = open(dest_name, O_CREAT | O_WRONLY | O_TRUNC, get_mode(source_name));
  int source_fd = open(source_name, O_RDONLY);
  int result = copy_regular_file(source_fd, dest_fd, get_size(source_name));
  close(dest_fd);
  close(source_fd);
  return result;
}

int copy_rec(char *path, char *dest) {
  char slash = '/';
  DIR *dir;
  struct dirent *ent;
  char *NullPositionPath = &path[strlen(path)];
  char *NullPositionDest = &dest[strlen(dest)];
  dir = opendir(path);
  if (dir == NULL) {
    fprintf(stderr, "Couldn't open %s directory", path);
    exit(EXIT_FAILURE);
  }
  mkdir(dest, get_mode(path));
  while ((ent = readdir(dir)) != NULL) {
    if (ent->d_type == DT_REG) {
      char dest_name[250];
      char source_name[250];
      sprintf(source_name, "%s%c%s", path, slash, ent->d_name);
      sprintf(dest_name, "%s%c%s", dest, slash, ent->d_name);
      if (copy_regular_file_names(source_name, dest_name) != 0) {
        fprintf(stderr, "Couldn't copy %s to %s", source_name, dest_name);
        exit(EXIT_FAILURE);
      }
    }
    if (ent->d_type == DT_DIR) {
      if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0)) {
        sprintf(NullPositionPath, "%c%s", slash, ent->d_name);
        sprintf(NullPositionDest, "%c%s", slash, ent->d_name);
        if (copy_rec(path, dest) != 0) {
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

int copy(char *path, char *dest, bool rec) {
  struct dirent *ent;
  char pathmax_path[255 + 1 + sizeof(ent->d_name) + 1];
  char pathmax_dest[255 + 1 + sizeof(ent->d_name) + 1];
  if (strlen(path) > 255) {
    return 1;
  }
  strcpy(pathmax_path, path);
  strcpy(pathmax_dest, dest);
  return copy_rec(pathmax_path, pathmax_dest);
}
