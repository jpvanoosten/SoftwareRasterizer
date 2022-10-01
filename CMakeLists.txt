cmake_minimum_required( VERSION 3.23.0 )

# Make sure DLL and EXE targets go to the same directory.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set( SR_VERSION_MAJOR 0 )
set( SR_VERSION_MINOR 0 )
set( SR_VERSION_PATCH 1 )
set( SR_VERSION ${SR_VERSION_MAJOR}.${SR_VERSION_MINOR}.${SR_VERSION_PATCH})



project( SoftwareRasterizer VERSION ${SR_VERSION} LANGUAGES CXX )

option( BUILD_SHARED_LIBS "Global flag to cause add_library to create shared libraries." ON )
option( SR_SAMPLES "Compile samples." OFF )

add_subdirectory(engine)

if(SR_SAMPLES)
    add_subdirectory(samples)
endif(SR_SAMPLES)