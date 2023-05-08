cmake_minimum_required(VERSION 3.25)
project(SystemProgramming01 C)

set(CMAKE_C_STANDARD 23)

find_package(SDL2 REQUIRED)

add_executable(SystemProgramming01 main.c )

target_link_libraries (SystemProgramming01 PRIVATE SDL2)