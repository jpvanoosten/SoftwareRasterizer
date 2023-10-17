cmake_minimum_required( VERSION 3.22.1 )

find_package( OpenGL REQUIRED )

add_executable( example_win32_opengl3 main.cpp )
target_link_libraries( example_win32_opengl3 PRIVATE imgui OpenGL::GL )