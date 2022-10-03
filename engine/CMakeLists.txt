cmake_minimum_required( VERSION 3.23.0 )

set( INC_FILES
    inc/Color.hpp
    inc/Events.hpp
    inc/Image.hpp
    inc/KeyCodes.hpp
    inc/Window.hpp
)

set( SRC_FILES
    src/Color.cpp
    src/Image.cpp
    src/Window.cpp
)

if(WIN32)
    list(APPEND INC_FILES
        inc/WindowWin32.hpp
    )

    list( APPEND SRC_FILES
        src/WindowWin32.cpp
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
    PUBLIC inc
)