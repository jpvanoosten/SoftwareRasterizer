cmake_minimum_required(VERSION 3.23.0) # The version of CMake used when creating this project.

include(FetchContent)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        0.9.9.8
)
FetchContent_Declare(
	LDtkLoader
	GIT_REPOSITORY https://github.com/Madour/LDtkLoader.git
	GIT_TAG		   1.1
)

message( STATUS "Fetching glm..." )
FetchContent_MakeAvailable(glm)

message( STATUS "Fetching LDtkLoader..." )
FetchContent_MakeAvailable(LDtkLoader)

if(BUILD_SHARED_LIBS)
set_target_properties(
	glm_shared LDtkLoader
    PROPERTIES FOLDER externals
)
endif(BUILD_SHARED_LIBS)