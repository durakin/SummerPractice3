cmake_minimum_required(VERSION 3.22)
project(Calculator C)

set(CMAKE_C_STANDARD 11)

add_executable(Calculator main.c priorities.c priorities.h calc.c calc.h)
