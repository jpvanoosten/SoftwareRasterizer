cmake_minimum_required(VERSION 3.23.0) # The version of CMake used when creating this project.

option(ALSOFT_UTILS "Build utility programs"  OFF)
option(ALSOFT_EXAMPLES  "Build example programs"  OFF)
option(ALSOFT_INSTALL "Install main library" OFF)
option(ALSOFT_INSTALL_CONFIG "Install alsoft.conf sample configuration file" OFF)
option(ALSOFT_INSTALL_HRTF_DATA "Install HRTF data files" OFF)
option(ALSOFT_INSTALL_AMBDEC_PRESETS "Install AmbDec preset files" OFF)
option(ALSOFT_INSTALL_EXAMPLES "Install example programs (alplay, alstream, ...)" OFF)
option(ALSOFT_INSTALL_UTILS "Install utility programs (openal-info, alsoft-config, ...)" OFF)
option(ALSOFT_UPDATE_BUILD_VERSION "Update git build version info" OFF)

include(FetchContent)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        0.9.9.8
)
FetchContent_Declare(
	LDtkLoader
	GIT_REPOSITORY https://github.com/jpvanoosten/LDtkLoader.git
	GIT_TAG		   master
)
FetchContent_Declare(
	glew
	GIT_REPOSITORY https://github.com/jpvanoosten/glew.git
)
FetchContent_Declare(
	OpenAL
	GIT_REPOSITORY https://github.com/kcat/openal-soft.git
	GIT_TAG 1.22.2
)

message( STATUS "Fetching glm..." )
FetchContent_MakeAvailable(glm)

message( STATUS "Fetching LDtkLoader..." )
FetchContent_MakeAvailable(LDtkLoader)

message( STATUS "Fetching glew..." )
FetchContent_MakeAvailable( glew )

message( STATUS "Fetching OpenAL..." )
FetchContent_MakeAvailable( OpenAL )

set_target_properties(
	LDtkLoader glew uninstall
    PROPERTIES FOLDER externals
)

set_target_properties(
	common ex-common OpenAL
    PROPERTIES FOLDER externals/OpenAL
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
