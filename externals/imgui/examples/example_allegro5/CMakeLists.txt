cmake_minimum_required( VERSION 3.22.1 )

add_executable( example_allegro5 imconfig_allegro5.h main.cpp )
target_link_libraries( example_allegro5 PRIVATE imgui )