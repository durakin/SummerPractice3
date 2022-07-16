#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include "newgui.h"
#include "ls.h"
#include "clipboard.h"


void free_mem(struct entry *entries, int entries_count) {
  for (int i = 0; i < entries_count; i++) {
    free(entries[i].name);
  }
}


void choose_dir(WINDOW* main_window, struct menu_context *context, char *name, char *full_name_buffer) {
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
  char** files_in_buffer_path = NULL;
  int files_in_buffer_count = 0;
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
  xMax/=2;
  WINDOW *left = newwin(yMax, xMax, 0, 0);
  WINDOW *right = newwin(yMax, xMax, 0, xMax);
  refresh();
  //box(left, 0, 0);
  //box(right, 0, 0);
  wrefresh(left);
  wrefresh(right);
  struct entry entries_left[10000];
  struct entry entries_right[10000];
  char full_name_buffer1[4096];
  char full_name_buffer2[4096];
  struct menu_context context1;
  struct menu_context context2;
  init_menu(&context1, NULL, entries_left, 0, 0, 1, 1, yMax - 2, xMax - 2);
  init_menu(&context2, NULL, entries_right, 0, 0, 1, xMax+1, yMax - 2, xMax - 2);
  choose_dir(left, &context1, ".", full_name_buffer1);
  choose_dir(right, &context2, ".", full_name_buffer2);
  refresh();
  wrefresh(context1.window);
  wrefresh(context2.window);
  struct menu_context* chosen_context = &context1;
  WINDOW* chosen_window = left;
  char* chosen_name_buffer = full_name_buffer1;
  struct entry *chosen_entries = entries_left;

  while (1) {
    c = wgetch(chosen_context->window);
    if (c == KEY_UP) {
      menu_up(chosen_context);
    }
    if (c == KEY_DOWN) {
      menu_down(chosen_context);
    }
    if (c == KEY_RIGHT) {
      if (chosen_entries[chosen_context->current_choice].type == DIRECTORY || chosen_entries[chosen_context->current_choice].type == UP_DIR) {
        char relative_path[4352];
        sprintf(relative_path, "%s/%s", chosen_name_buffer, chosen_entries[chosen_context->current_choice].name);
        choose_dir(chosen_window, chosen_context, relative_path, chosen_name_buffer);
      }
      if (chosen_entries[chosen_context->current_choice].type == EXECUTABLE) {
        // TODO: get params and run
      }
    }
    if (c == KEY_LEFT) {
      char relative_path[4352];
      sprintf(relative_path, "%s/%s", chosen_name_buffer, "..");
      choose_dir(chosen_window, chosen_context, relative_path, chosen_name_buffer);
    }
    if (c == KEY_F(1)) {
      if (chosen_entries[chosen_context->current_choice].type != UP_DIR) {
        char relative_path[4352];
        sprintf(relative_path, "%s/%s", chosen_name_buffer, entries_left[chosen_context->current_choice].name);
        files_in_buffer_path = handle_buffer(files_in_buffer_path, relative_path, &files_in_buffer_count);
      }
    }
    if (c == KEY_F(2)) {
      FILE *out_file = fopen("buffer", "w"); // write only
      for (int i = 0; i < files_in_buffer_count; i++) {
        fprintf(out_file, "%s\n", files_in_buffer_path[i]);
      }
      fclose(out_file);
    }
    if (c == KEY_SRIGHT) {
      chosen_window = right;
      chosen_context = &context2;
      chosen_entries = entries_right;
      chosen_name_buffer= full_name_buffer2;
    }
    if (c == KEY_SLEFT) {
      chosen_window = left;
      chosen_context = &context1;
      chosen_entries = entries_left;
      chosen_name_buffer= full_name_buffer1;
    }
    if (c == KEY_F(10)) {
      break;
    }
  }
  endwin();
  free_buffer(files_in_buffer_path, files_in_buffer_count);
  files_in_buffer_path = NULL;
  files_in_buffer_count = 0;
  return 0;
}
