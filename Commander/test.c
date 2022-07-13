#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "newgui.h"
#include "ls.h"

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
  WINDOW *main = newwin(yMax, xMax, 0, 0);
  refresh();
  box(main, 0, 0);
  wrefresh(main);
  getch();
  struct entry entries[100];
  char full_name_buffer[4096];
  int entries_count = ls(entries, "../../");
  struct menu_context context;
  init_menu(&context, full_name_buffer, entries, entries_count, 3, 1, 1, yMax - 2, xMax - 2);
  refresh();
  wrefresh(context.window);
  getch();
  print_list(&context);

  while (1) {
    c = wgetch(context.window);
    if (c == KEY_UP) {
      menu_up(&context);
    }
    if (c == KEY_DOWN) {
      menu_down(&context);
    }
    /*if (c == 'F' || c == 'f') {
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
     */
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
  endwin();
  return 0;
}
