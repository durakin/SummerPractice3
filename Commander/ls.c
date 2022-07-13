#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include "ls.h"

mode_t get_mode(char *realpath_buff) {
  struct stat stat_buff;
  stat(realpath_buff, &stat_buff);
  return stat_buff.st_mode;
}

bool is_directory(char *realpath_buff) {
  return S_ISDIR(get_mode(realpath_buff));
}

bool is_executable(char *realpath_buff) {
  return (get_mode(realpath_buff) & S_IXUSR);
}

bool is_regular(char *realpath_buff) {
  return S_ISREG(get_mode(realpath_buff));
}

int comparator(const void *a, const void *b) {
  struct entry entry1 = *(struct entry *) a;
  struct entry entry2 = *(struct entry *) b;
  return (entry1.type - entry2.type);
}

int ls(struct entry *buffer, char *full_name) {
  DIR *directory;
  struct dirent *current_dirent;
  directory = opendir(full_name);
  int counter = 0;
  char realpath_buffer[4096];
  realpath_buffer[0] = '\0';
  strcpy(realpath_buffer, full_name);

  while ((current_dirent = readdir(directory)) != NULL) {
    struct entry new_entry;
    sprintf(realpath_buffer + strlen(full_name), "/%s%c", current_dirent->d_name, '\0');
    new_entry.size = get_size(realpath_buffer);
    new_entry.name = malloc(strlen(current_dirent->d_name));
    strcpy(new_entry.name, current_dirent->d_name);

    if (strcmp(new_entry.name, ".") == 0) {
      continue;
    }
    if (strcmp(new_entry.name, "..") == 0) {
      new_entry.type = UP_DIR;
    } else if (is_directory(realpath_buffer)) {
      new_entry.type = DIRECTORY;
    } else if (is_executable(realpath_buffer)) {
      new_entry.type = EXECUTABLE;
    } else if (is_regular(realpath_buffer)) {
      new_entry.type = STANDARD;
    }
    // TODO: other files must be indexed too
    else continue;
    buffer[counter] = new_entry;
    counter++;
  }
  closedir(directory);
  qsort(buffer, counter, sizeof(struct entry), comparator);
  return counter;
}

size_t get_size(char *realpath_buff) {
  struct stat stat_buff;
  stat(realpath_buff, &stat_buff);
  return stat_buff.st_size;
}