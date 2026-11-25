cmake_minimum_required(VERSION 3.30)
project(projet_allegro2 C)

set(CMAKE_C_STANDARD 23)

add_executable(projet_allegro2 main.c)
target_link_libraries(projet_allegro2 -lmingw32 -lalleg44)