cmake_minimum_required( VERSION 3.23.0 )

set( SRC_FILES
    Engine.cpp
    Window.cpp
)

set( INC_FILES
    Engine.hpp
    Window.hpp
)

if(WIN32)
    list( APPEND SRC_FILES
        WindowWin32.cpp
    )

    list(APPEND INC_FILES
        WindowWin32.hpp
    )
endif(WIN32)

set( ALL_FILES ${SRC_FILES} ${INC_FILES} )

add_library( Engine ${ALL_FILES} )

set_target_properties( Engine
    PROPERTIES
        CXX_STANDARD 20
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions( Engine
        PRIVATE Engine_EXPORTS
        INTERFACE Engine_IMPORTS
    )
endif(BUILD_SHARED_LIBS)

target_include_directories( Engine
    PUBLIC .
)