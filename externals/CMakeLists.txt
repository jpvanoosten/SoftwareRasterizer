cmake_minimum_required(VERSION 3.23.0) # The version of CMake used when creating this project.

include(FetchContent)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        0.9.9.8
)
FetchContent_Declare(
	glew
	GIT_REPOSITORY https://github.com/jpvanoosten/glew.git
)

message( STATUS "Fetching glm..." )
FetchContent_MakeAvailable(glm)

message( STATUS "Fetching glew..." )
FetchContent_MakeAvailable( glew )

set_target_properties(
	glew uninstall
    PROPERTIES FOLDER externals
)

if(BUILD_SHARED_LIBS)
set_target_properties(
	glm_shared
    PROPERTIES FOLDER externals
)
else()
set_target_properties(
	glm
    PROPERTIES FOLDER externals
)
endif(BUILD_SHARED_LIBS)
