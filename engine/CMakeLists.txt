cmake_minimum_required( VERSION 3.23.0 )

set( INC_FILES
    inc/aligned_unique_ptr.hpp
    inc/BlendMode.hpp
    inc/Config.hpp
    inc/Color.hpp
    inc/Enums.hpp
    inc/Events.hpp
    inc/Font.hpp
    inc/Image.hpp
    inc/KeyCodes.hpp
    inc/Sprite.hpp
    inc/stb_easy_font.h
    inc/stb_image.h
	inc/stb_truetype.h
    inc/Timer.hpp
    inc/Vertex.hpp
    inc/Window.hpp
)

set( SRC_FILES
    src/BlendMode.cpp
    src/Color.cpp
    src/Font.cpp
    src/Image.cpp
    src/stb_image.cpp
	src/stb_truetype.cpp
    src/Timer.cpp
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

set( ALL_FILES 
    ${SRC_FILES} 
    ${INC_FILES} 
    ../.clang-format
)

add_library( SoftwareRasterizer ${ALL_FILES} )

set_target_properties( SoftwareRasterizer
    PROPERTIES
        CXX_STANDARD 20
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions( SoftwareRasterizer
        PRIVATE SoftwareRasterizer_EXPORTS
        INTERFACE SoftwareRasterizer_IMPORTS
    )
endif(BUILD_SHARED_LIBS)

target_include_directories( SoftwareRasterizer
    PUBLIC inc
)

target_link_libraries( SoftwareRasterizer
    PUBLIC Math
)

find_package(OpenMP)
if(OpenMP_FOUND)
    target_include_directories( SoftwareRasterizer
        PRIVATE ${OpenMP_CXX_INCLUDE_DIRS}
    )

    target_link_libraries( SoftwareRasterizer
        PUBLIC OpenMP::OpenMP_CXX
    )
endif(OpenMP_FOUND)