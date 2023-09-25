cmake_minimum_required( VERSION 3.22.1 )

set( INC_FILES
    inc/Math/AABB.hpp
    inc/Math/bitmask_operators.hpp
    inc/Math/Camera2D.hpp
    inc/Math/Camera3D.hpp
    inc/Math/Circle.hpp
    inc/Math/Line.hpp
    inc/Math/Math.hpp
    inc/Math/OutCodes.hpp
    inc/Math/Rect.hpp
    inc/Math/Space.hpp
    inc/Math/Sphere.hpp
    inc/Math/Transform2D.hpp
)

set( SRC_FILES
    src/Camera2D.cpp
    src/Camera3D.cpp
    src/Math.cpp
    src/Transform2D.cpp
)

set( ALL_FILES 
    ${SRC_FILES} 
    ${INC_FILES} 
    ../.clang-format
)

add_library( Math STATIC ${ALL_FILES} )

set_target_properties( Math
    PROPERTIES
        CXX_STANDARD 20
)

target_include_directories( Math
    PUBLIC inc
)

target_link_libraries( Math
    PUBLIC glm
)

install(TARGETS Math)
install( DIRECTORY inc/Math DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )
