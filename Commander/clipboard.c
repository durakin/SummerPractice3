#include <string.h>
#include <stdlib.h>

#include "clipboard.h"
void free_buffer(char **buffer, int buffer_counter) {
  for (int i = 0; i < buffer_counter; i++) {
    free(buffer[i]);
  }
  free(buffer);
}

char **file_to_buffer(char **buffer, char *realpath, int *buffer_counter) {
  buffer = (char **) realloc(buffer, sizeof(char *) * ((*buffer_counter) + 1));
  buffer[*buffer_counter] = (char *) malloc(strlen(realpath) + 1);
  strcpy(buffer[*buffer_counter], realpath);
  (*buffer_counter)++;
  return buffer;
}

char **file_from_buffer(char **buffer, int file_in_buffer_index, int *buffer_counter) {
  free(buffer[file_in_buffer_index]);
  buffer[file_in_buffer_index] = buffer[*buffer_counter - 1];
  (*buffer_counter)--;
  return (char **) realloc(buffer, sizeof(char *) * ((*buffer_counter)));
}

char **handle_buffer(char **buffer, char *path, int *buffer_counter) {
  char realpath_buffer[4096];
  int file_in_buffer_index = -1;
  realpath(path, realpath_buffer);
  for (int i = 0; i < *buffer_counter; i++) {
    if (strcmp(buffer[i], realpath_buffer) == 0) {
      file_in_buffer_index = i;
      break;
    }
  }
  if (file_in_buffer_index != -1) {
    return file_from_buffer(buffer, file_in_buffer_index, buffer_counter);
  } else return file_to_buffer(buffer, realpath_buffer, buffer_counter);
}
