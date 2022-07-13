#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include "gui.h"

void print_entry(WINDOW *menu_win, struct entry entry_to_print, int offset_y, int offset_x, int add_attrs) {
  char formats[4][10] = {"/%s %d", "/%s %d", "*%s %d", " %s %d"};
  int attrs[4] = {A_BOLD, A_BOLD, COLOR_PAIR(1), A_NORMAL};
  wattron(menu_win, attrs[entry_to_print.type] | add_attrs);
  mvwprintw(menu_win, offset_y, offset_x, formats[entry_to_print.type], entry_to_print.name, entry_to_print.size);
  wrefresh(menu_win);
  wattroff(menu_win, attrs[entry_to_print.type] | add_attrs);
}

void print_directory(WINDOW *menu_win,
                     char *title_string,
                     int start_y,
                     int start_x,
                     struct entry *entries,
                     int entries_count) {
  wclear(menu_win);
  box(menu_win, 0, 0);
  mvwaddstr(menu_win, 0, 1, title_string);
  wrefresh(menu_win);
  for (int i = 0; i < entries_count; i++) {
    print_entry(menu_win, entries[i], start_y + i, start_x, (i == 0) ? A_STANDOUT : 0);
  }
}

void menu_choose(WINDOW *menu_win,
                 int *current_menu_position,
                 int start_y,
                 int start_x,
                 struct entry *entries,
                 int new_menu_position,
                 int max_menu_position) {
  if (!(0 <= new_menu_position && new_menu_position < max_menu_position)) return;
  print_entry(menu_win, entries[*current_menu_position], start_y + *current_menu_position, start_x, 0);
  (*current_menu_position) = new_menu_position;
  print_entry(menu_win, entries[*current_menu_position], start_y + *current_menu_position, start_x, A_STANDOUT);
  wrefresh(menu_win);
}

void menu_up(WINDOW *menu_win, int *current_menu_position, int start_y, int start_x, struct entry *entries) {
  if (*current_menu_position > 0) {
    menu_choose(menu_win,
                current_menu_position,
                start_y,
                start_x,
                entries,
                *current_menu_position - 1,
                *current_menu_position + 1);
  }
}

void menu_down(WINDOW *menu_win,
               int *current_menu_position,
               int start_y,
               int start_x,
               struct entry *entries,
               int max_entries) {
  if (*current_menu_position < max_entries - 1) {
    menu_choose(menu_win, current_menu_position, start_y, start_x, entries, *current_menu_position + 1, max_entries);
  }
}
