#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "gui.h"
#include "ls.h"

void free_mem(struct entry *entries, int entries_count) {
  for (int i = 0; i < entries_count; i++) {
    free(entries[i].name);
  }
}

void choose_dir(WINDOW *menu_win, struct entry *entries, int *entries_count, char *name, char *full_name_buffer) {
  free_mem(entries, *entries_count);
  realpath(name, full_name_buffer);
  *entries_count = ls(entries, full_name_buffer);
  print_directory(menu_win, full_name_buffer, 1, 1, entries, *entries_count);
  refresh();
}


int main() {
  int choice = 0;
  int c;
  initscr();
  clear();
  noecho();
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
  int entries_count;

  choose_dir(menu_win, entries, &entries_count, ".", full_name_buffer);

  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
  while (1) {
    c = wgetch(menu_win);
    if (c == KEY_UP) {
      menu_up(menu_win, &choice, 1, 1, entries);
    }
    if (c == KEY_DOWN) {
      menu_down(menu_win, &choice, 1, 1, entries, entries_count);
    }
    if (c == 'F' || c == 'f') {
      if (entries[choice].type == DIRECTORY || entries[choice].type == UP_DIR) {
        char relative_path[4352];
        sprintf(relative_path, "%s/%s", full_name_buffer, entries[choice].name);
        choice = 0;
        choose_dir(menu_win, entries, &entries_count, relative_path, full_name_buffer);
      }
      if (entries[choice].type == EXECUTABLE) {
        // TODO: get params and run
      }
    }
    /*if (c == KEY_MOUSE) {
      if (getmouse(&event) == OK) {
        if (BUTTON1_CLICKED && event.y >= 1) {
          menu_choose(menu_win, &choice, 1, 1, entries, event.y - 1, entries_count);
        } else if (BUTTON1_DOUBLE_CLICKED) {
          char relative_path[4352];
          sprintf(relative_path, "%s/%s", full_name_buffer, entries[choice].name);
          choice = 0;
          choose_dir(menu_win, entries, &entries_count, relative_path, full_name_buffer);
        }
      }
    }
     */
    if (c == KEY_F(10)) {
      break;
    }
  }
  free_mem(entries, entries_count);
  endwin();
}
