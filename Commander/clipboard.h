#ifndef COMMANDER__CLIPBOARD_H_
#define COMMANDER__CLIPBOARD_H_

void free_buffer(char **buffer, int buffer_counter);

char **file_to_buffer(char **buffer, char *realpath, int *buffer_counter);

char **handle_buffer(char **buffer, char *path, int *buffer_counter);

#endif //COMMANDER__CLIPBOARD_H_
