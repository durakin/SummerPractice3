cmake_minimum_required(VERSION 3.16)
project(Commander C)

set(CMAKE_C_STANDARD 11)

add_executable(Commander main.c ls.c ls.h gui.c gui.h)
add_executable(Test test.c ls.c ls.h newgui.c newgui.h)

target_link_libraries(Commander PRIVATE ncurses menu)
target_link_libraries(Test PRIVATE ncurses menu)
