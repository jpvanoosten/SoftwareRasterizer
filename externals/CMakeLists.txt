cmake_minimum_required(VERSION 3.22.1) # The version of CMake used when creating this project.

include(FetchContent)

add_subdirectory(glad)
add_subdirectory(tinyobjloader)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        0.9.9.8
)

FetchContent_Declare (
	fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt.git
	GIT_TAG        9.1.0
)

message( STATUS "Fetching glm..." )
FetchContent_MakeAvailable(glm)

message( STATUS "Fetching fmt..." )
FetchContent_MakeAvailable(fmt)

set_target_properties(
	fmt glad tinyobjloader
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
