#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include "newgui.h"
#include "ls.h"

void free_mem(struct entry *entries, int entries_count) {
  for (int i = 0; i < entries_count; i++) {
    free(entries[i].name);
  }
}

void choose_dir(WINDOW* main_window, struct menu_context *context, char *name, char *full_name_buffer) {
  //int statfs(const char *path, struct statfs *buf);

  free_mem(context->entries, context->entry_count);
  realpath(name, full_name_buffer);
  struct statvfs stat;
  statvfs(full_name_buffer, &stat);
  context->entry_count = ls(context->entries, full_name_buffer);
  context->current_choice = 0;
  context->first_visible = 0;
  char new_window_name[4096];
  get_beautiful_name(full_name_buffer, getmaxx(main_window) - 2, new_window_name);
  box(main_window, 0, 0);
  mvwprintw(main_window, 0, 1, new_window_name);
  char space[12];
  convert_size(stat.f_bsize * stat.f_blocks, space);
  char freespace[12];
  convert_size( stat.f_bsize * stat.f_bfree, freespace);
  mvwprintw(main_window, context->max_y+1, 1, "%s/%s", freespace, space);
  wrefresh(main_window);
  print_list(context);
}


int main() {
  int chosen_tool;
  char chosen_file_name[4096];
  int c;
  initscr();
  clear();
  noecho();
  cbreak();
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  int y, x, yMax, xMax;
  getyx(stdscr, y, x);
  getmaxyx(stdscr, yMax, xMax);
  WINDOW *main = newwin(yMax, xMax, 0, 0);
  refresh();
  box(main, 0, 0);
  wrefresh(main);
  struct entry entries[10000];
  char full_name_buffer[4096];
  struct menu_context context;
  init_menu(&context, NULL, entries, 0, 0, 1, 1, yMax - 2, xMax - 2);
  choose_dir(main, &context, ".", full_name_buffer);
  refresh();
  wrefresh(context.window);
  print_list(&context);

  while (1) {
    c = wgetch(context.window);
    if (c == KEY_UP) {
      menu_up(&context);
    }
    if (c == KEY_DOWN) {
      menu_down(&context);
    }
    if (c == KEY_RIGHT) {
      if (entries[context.current_choice].type == DIRECTORY || entries[context.current_choice].type == UP_DIR) {
        char relative_path[4352];
        sprintf(relative_path, "%s/%s", full_name_buffer, entries[context.current_choice].name);
        choose_dir(main, &context, relative_path, full_name_buffer);
      }
      if (entries[context.current_choice].type == EXECUTABLE) {
        // TODO: get params and run
      }
    }
    if (c == KEY_LEFT) {
      char relative_path[4352];
      sprintf(relative_path, "%s/%s", full_name_buffer, "..");
      choose_dir(main, &context, relative_path, full_name_buffer);
    }
    if (c == KEY_F(1)) {
      if (entries[context.current_choice].type != UP_DIR) {
        char relative_path[4352];
        sprintf(relative_path, "%s/%s", full_name_buffer, entries[context.current_choice].name);
        realpath(relative_path, chosen_file_name);
      }
    }
    if (c == KEY_F(10)) {
      break;
    }
  }
  endwin();
  return 0;
}
