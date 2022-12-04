cmake_minimum_required( VERSION 3.23.0 )

set( INC_FILES
    inc/Math/AABB.hpp
    inc/Math/Math.hpp
)

set( SRC_FILES
    src/Math.cpp
)

set( ALL_FILES 
    ${SRC_FILES} 
    ${INC_FILES} 
    ../.clang-format
)

add_library( Math STATIC ${ALL_FILES} )

target_include_directories( Math
    PUBLIC inc
)

target_link_libraries( Math
    PUBLIC glm
)