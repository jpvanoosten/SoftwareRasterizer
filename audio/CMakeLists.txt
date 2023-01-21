cmake_minimum_required( VERSION 3.23.0 )

set( INC_FILES
)

set( SRC_FILES
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
        PRIVATE SoftwareRasterizer_EXPORTS
        INTERFACE SoftwareRasterizer_IMPORTS
    )
endif(BUILD_SHARED_LIBS)

target_include_directories( Audio
    PUBLIC inc
)

target_link_libraries( Audio
    PUBLIC Math 
    PRIVATE glew
)
