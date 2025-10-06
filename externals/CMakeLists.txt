cmake_minimum_required(VERSION 3.22.1) # The version of CMake used when creating this project.

option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option( IMGUI_BACKEND_GLFW "Add backend files for GLFW." ON )
option( IMGUI_BACKEND_OPENGL3 "Add backend files for OpenGL 3.0+" ON )

include(FetchContent)

add_subdirectory(glad)
add_subdirectory(tinyobjloader)
add_subdirectory(imgui)

FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 	   3.4
)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        1.0.1
)

FetchContent_Declare (
	fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt.git
	GIT_TAG        12.0.0
)

message( STATUS "Fetching glfw..." )
FetchContent_MakeAvailable(glfw)

message( STATUS "Fetching glm..." )
FetchContent_MakeAvailable(glm)

message( STATUS "Fetching fmt..." )
FetchContent_MakeAvailable(fmt)

target_link_libraries( imgui
	PUBLIC glfw
)

set_target_properties(
	fmt glad imgui tinyobjloader
    PROPERTIES FOLDER externals
)

set_target_properties(
	glfw update_mappings
	PROPERTIES FOLDER externals/GLFW
)

if(TARGET glm_shared)
set_target_properties(
	glm_shared
    PROPERTIES FOLDER externals
)
endif()

if(TARGET glm)
set_target_properties(
	glm
    PROPERTIES FOLDER externals
)
endif()
