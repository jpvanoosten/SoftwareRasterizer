cmake_minimum_required( VERSION 3.23.0 )

option( BUILD_SHARED_LIBS "Global flag to cause add_library to create shared libraries." ON )
option( SR_BUILD_SAMPLES "Build samples." ON )

# Make sure DLL and EXE targets go to the same directory.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set( SR_VERSION_MAJOR 0 )
set( SR_VERSION_MINOR 0 )
set( SR_VERSION_PATCH 1 )
set( SR_VERSION ${SR_VERSION_MAJOR}.${SR_VERSION_MINOR}.${SR_VERSION_PATCH})

# Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project( SoftwareRasterizer VERSION ${SR_VERSION} LANGUAGES CXX )

# Enable multi-processor builds in MSVC.
if(MSVC)
    add_compile_options(/MP)
endif(MSVC)

add_subdirectory(externals)
add_subdirectory(math)
add_subdirectory(audio)
add_subdirectory(engine)

if(SR_BUILD_SAMPLES)
    add_subdirectory(samples)
    # Set the startup project.
    set_directory_properties( PROPERTIES
        VS_STARTUP_PROJECT 02-Triangle
    )
endif(SR_BUILD_SAMPLES)