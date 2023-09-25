cmake_minimum_required(VERSION 3.22.1) # The version of CMake used when creating this project.

add_library( tinyobjloader  STATIC tiny_obj_loader.h tiny_obj_loader.cc )

target_include_directories( tinyobjloader
    PUBLIC .
)
