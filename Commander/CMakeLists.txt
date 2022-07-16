cmake_minimum_required(VERSION 3.16)
project(Commander C)

set(CMAKE_C_STANDARD 11)

add_executable(Commander main.c ls.c ls.h newgui.c newgui.h clipboard.c clipboard.h)

target_link_libraries(Commander PRIVATE ncurses menu)
