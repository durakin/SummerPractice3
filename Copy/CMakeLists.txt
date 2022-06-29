cmake_minimum_required(VERSION 3.16)
project(Copy C)

set(CMAKE_C_STANDARD 11)

add_executable(Copy main.c copy.c copy.h)
