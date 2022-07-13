#ifndef COMMANDER__GUI_H_
#define COMMANDER__GUI_H_
#include "ls.h"
#include <ncurses.h>

void print_directory(WINDOW *menu_win, char *title_string,
                     int start_y,
                     int start_x,
                     struct entry *entries,
                     int entries_count);

void menu_choose(WINDOW *menu_win,
                 int *current_menu_position,
                 int start_y,
                 int start_x,
                 struct entry *entries,
                 int new_menu_position,
                 int max_menu_position);

void menu_up(WINDOW *menu_win, int *current_menu_position, int start_y, int start_x, struct entry *entries);

void menu_down(WINDOW *menu_win,
               int *current_menu_position,
               int start_y,
               int start_x,
               struct entry *entries,
               int max_entries);

#endif //COMMANDER__GUI_H_
