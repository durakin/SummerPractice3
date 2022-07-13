#include <ncurses.h>
#include <stdlib.h>
#include "ls.h"

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
  box(menu_win, 0, 0);
  //mvwprintw(menu_win, 0, 1, "%3.s", full_name_buffer);
  mvwaddstr(menu_win, 0, 1, title_string);
  //refresh();
  wrefresh(menu_win);
  //struct entry entries[100];
  //int entries_count = ls(entries, full_name_buffer);
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

int main() {
  int choice = 0;
  int c;
  initscr();
  clear();
  noecho();
  cbreak();
  start_color();
  MEVENT event;
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  int y, x, yMax, xMax;
  getyx(stdscr, y, x);
  getmaxyx(stdscr, yMax, xMax);
  WINDOW *menu_win = newwin(yMax, xMax, y, x);
  keypad(menu_win, TRUE);
  refresh();
  struct entry entries[100];
  char full_name_buffer[4096];
  realpath(".", full_name_buffer);
  int entries_count = ls(entries, full_name_buffer);
  print_directory(menu_win, full_name_buffer, 1, 1, entries, entries_count);
  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
  while (1) {
    c = wgetch(menu_win);
    if (c == KEY_UP) {
      menu_up(menu_win, &choice, 1, 1, entries);
    }
    if (c == KEY_DOWN) {
      menu_down(menu_win, &choice, 1, 1, entries, entries_count);
    }
    if (c == KEY_MOUSE) {
      if (getmouse(&event) == OK) {
        if (BUTTON1_PRESSED && event.y >= 1) {
          menu_choose(menu_win, &choice, 1, 1, entries, event.y - 1, entries_count);
        }
      }
    }
    if (c == KEY_F(10)) {
      break;
    }
  }
  for (int i = 0; i < entries_count; i++) {
    free(entries[i].name);
  }
  endwin();
}
