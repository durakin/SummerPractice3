#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "newgui.h"

void convert_size(size_t size, char* buffer) {
  char units[5][4] = { "B", "KiB", "MiB", "GiB", "TiB" };
  int unit_index = 0;
  double converted_size;
  converted_size = size;
  while (converted_size / 1024 > 1 && unit_index <= 4) {
    unit_index++;
    converted_size/=1024.0;
  }
  sprintf(buffer, "%.2f %s", converted_size, units[unit_index]);
}

void get_beautiful_name(char* path, int max_length, char* buffer) {
  bool in_home = false;
  int length = strlen(path);
  char* homedir = getenv("HOME");
  if (homedir!=NULL) {
    if (strncmp(homedir, path, strlen(homedir)) == 0 && path[strlen(homedir)] == '/') {
      length = 1 + length - strlen(homedir);
      in_home = true;
    }
  }
  if (length <= max_length) {
    if (in_home) sprintf(buffer, "~%s", path+strlen(homedir));
    else sprintf(buffer, "%s", path);
    return;
  }
  else if (max_length > 2) {
    sprintf(buffer, "..%s", path+strlen(path) - max_length + 2);
    return;
  }
}

bool is_beyond_list(struct menu_context* context, int index) {
  return index - context->first_visible >= context->max_y;
}

void init_menu(struct menu_context* this, char* dir_path, struct entry* entries, int entry_count, int current_choice, int start_y, int start_x,
    int max_y, int max_x) {
  this->window = newwin(max_y, max_x, start_y, start_x);
  keypad(this->window, TRUE);
  this->start_x = start_x;
  this->start_y = start_y;
  this->max_x = max_x;
  this->max_y = max_y;
  this->entries = entries;
  this->entry_count = entry_count;
  this->current_choice = current_choice;
  this->first_visible = 0;
}

void cut_name(char* name, int max_length, char* buffer) {
  if (strlen(name) <= max_length) {
    sprintf(buffer, "%s", name);
    return;
  }
  sprintf(buffer, "..%s", name + strlen(name) - max_length +2);
}

void prepare_entry_string(struct entry entry_to_print, int max_length, char* buffer) {
  char appends[4] = {'/', '/', '*', ' '};
  char format[40];
  int name_length = max_length - 14;
  sprintf(format, "%c%%-%ds%%-%ds\n", appends[entry_to_print.type], name_length, 14);
  char size[14];
  char name_buff[255];
  convert_size(entry_to_print.size, size);
  cut_name(entry_to_print.name, max_length-15, name_buff);
  sprintf(buffer, format, name_buff, size);
}

void print_entry(struct menu_context* context, int index) {
  int attrs[4] = {A_BOLD, A_BOLD, COLOR_PAIR(1), A_NORMAL};
  struct entry entry_to_print = context->entries[index];
  WINDOW* window = context->window;
  int add_attr = (context->current_choice == index) ? A_STANDOUT : 0;
  wattron(context->window, attrs[entry_to_print.type] | add_attr);
  char entry_string[255];
  prepare_entry_string(entry_to_print, context->max_x-2, entry_string);
  mvwprintw(window, index-context->first_visible, 0, "%s", entry_string);
  wattroff(window, attrs[entry_to_print.type] | add_attr);
}

void print_list(struct menu_context* context) {
  WINDOW* window = context->window;
  wclear(window);
  for (int i = context->first_visible; i < context->entry_count && !is_beyond_list(context, i); i++) {
    print_entry(context, i);
  }
  wrefresh(window);
}

void menu_up(struct menu_context* context) {
  if (context->current_choice == 0) return;
  context->current_choice--;
  if (context->current_choice + 1 == context->first_visible) {
    context->first_visible = context->current_choice;
    print_list(context);
  }
  print_entry(context, context->current_choice);
  print_entry(context, context->current_choice+1);
  wrefresh(context->window);
}

void menu_down(struct menu_context* context) {
  if (context->current_choice == context->entry_count-1) return;
  context->current_choice++;
  if (is_beyond_list(context, context->current_choice)) {
    context->first_visible++;
    print_list(context);
  }
  print_entry(context, context->current_choice);
  print_entry(context, context->current_choice-1);
  wrefresh(context->window);
}