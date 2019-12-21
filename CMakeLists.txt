cmake_minimum_required(VERSION 3.0)

project(Editor C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -pedantic -Wextra")

add_subdirectory(src)
