#ifndef COMMANDER__NEWGUI_H_
#define COMMANDER__NEWGUI_H_
#include <ncurses.h>
#include "ls.h"
struct menu_context {
  WINDOW* window;
  char* dir_path;
  struct entry* entries;
  int entry_count;
  int current_choice;
  int start_y;
  int start_x;
  int max_y;
  int max_x;
  int first_visible;
};

void init_menu(struct menu_context* this, char* dir_path, struct entry* entries, int entry_count, int current_choice, int start_y, int start_x,
               int max_y, int max_x);

void print_list(struct menu_context* context);

void menu_up(struct menu_context* context);

void menu_down(struct menu_context* context);


#endif //COMMANDER__NEWGUI_H_
