#include <stdio.h>
#include <ncurses.h>
#include <dirent.h>
#include <stdlib.h>
#include "ls.h"

void print_explorer(WINDOW *menu_win, char* name, int start_y, int start_x) {
  box(menu_win, 0, 0);
  char full_name_buffer[4096];
  realpath(name, full_name_buffer);
  struct entry entries[100];
  int entries_count = ls(entries, full_name_buffer);
  for (int i = 0; i < entries_count; i++) {
    mvwprintw(menu_win, start_y+i, start_x, "%s %d %d", entries[i].name, entries[i].size, entries[i].type);
  }
  wrefresh(menu_win);
}



int main() {
  WINDOW *main_window;
  int highlight = 1;
  int choice = 0;
  int c;

  initscr();
  clear();
  noecho();
  cbreak();
  int y, x, yBeg, xBeg, yMax, xMax;
  getyx(stdscr, y, x);
  getmaxyx(stdscr, yMax, xMax);


  WINDOW *menu_win = newwin(yMax, xMax, y, x);
  keypad(menu_win, TRUE);
  refresh();
  //wrefresh(menu_win);
  //while (1) {
  //  c = wgetch(menu_win);
  //  switch (c) {
  //    case KEY_UP:
  //      if (highlight == 1)
  //        highlight = n_choices;
  //      else
  //        --highlight;
  //      break;
  //    case KEY_DOWN:
  //      if (highlight == n_choices)
  //        highlight = 1;
  //      else
  //        ++highlight;
  //      break;
  //    case 10:choice = highlight;
  //      break;
  //    default:mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", c, c);
  //      refresh();
  //      break;
  //  }
    print_explorer(menu_win, ".", 1, 1);
    getch();
    endwin();
  }

