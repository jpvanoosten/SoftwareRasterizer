cmake_minimum_required( VERSION 3.23.0 )

set( INC_FILES
    inc/Config.hpp
    inc/Device.hpp
)

set( SRC_FILES
    src/Device.cpp
    src/miniaudio.c
    src/miniaudio.h
)

set( ALL_FILES 
    ${SRC_FILES} 
    ${INC_FILES} 
    ../.clang-format
)

add_library( Audio ${ALL_FILES} )

set_target_properties( Audio
    PROPERTIES
        CXX_STANDARD 20
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions( Audio
        PRIVATE Audio_EXPORTS
        INTERFACE Audio_IMPORTS
    )
endif(BUILD_SHARED_LIBS)

target_include_directories( Audio
    PUBLIC inc
)
