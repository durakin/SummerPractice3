#include "newgui.h"

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

void print_entry(struct menu_context* context, int index) {
  //TODO: need to cut name depending on max_x
  char formats[4][10] = {"/%s %d", "/%s %d", "*%s %d", " %s %d"};
  int attrs[4] = {A_BOLD, A_BOLD, COLOR_PAIR(1), A_NORMAL};
  struct entry entry_to_print = context->entries[index];
  WINDOW* window = context->window;
  int add_attr = (context->current_choice == index) ? A_STANDOUT : 0;
  wattron(context->window, attrs[entry_to_print.type] | add_attr);
  mvwprintw(window, index-context->first_visible, 0, formats[entry_to_print.type], entry_to_print.name, entry_to_print.size);
  wrefresh(window);
  wattroff(window, attrs[entry_to_print.type] | add_attr);
}

void print_list(struct menu_context* context) {
  WINDOW* window = context->window;
  wclear(window);
  for (int i = context->first_visible; i < context->entry_count && !is_beyond_list(context, i); i++) {
    print_entry(context, i);
  }
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
}