cmake_minimum_required(VERSION 3.16)
project(Calculator C)

set(CMAKE_C_STANDARD 11)

add_executable(Calculator main.c priorities.c priorities.h calc.c calc.h)
add_executable(CalculatorTest Test/main.c calc.h calc.c priorities.h priorities.c)
target_link_libraries(CalculatorTest cunit)
target_link_libraries(CalculatorTest cunit m)
