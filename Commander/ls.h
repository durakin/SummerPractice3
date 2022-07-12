#ifndef COMMANDER__LS_H_
#define COMMANDER__LS_H_
enum {
  UP_DIR = 0,
#define UP_DIR UP_DIR
  DIRECTORY = 1,
#define DIRECTORY DIRECTORY
  EXECUTABLE = 2,
#define EXECUTABLE EXECUTABLE
  STANDARD = 3,
#define STANDARD STANDARD
};

struct entry {
  size_t size;
  char* name;
  int type;
};

int ls(struct entry *buffer, char *full_name);

size_t get_size(char* realpath_buff);

#endif //COMMANDER__LS_H_
