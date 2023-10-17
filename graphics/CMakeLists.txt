cmake_minimum_required( VERSION 3.22.1 )

option(SR_USE_OPENMP "Use OpenMP for parallel loops." OFF)

set( INC_FILES
    inc/Graphics/BlendMode.hpp
    inc/Graphics/Config.hpp
    inc/Graphics/Color.hpp
    inc/Graphics/Enums.hpp
    inc/Graphics/Events.hpp
    inc/Graphics/File.hpp
    inc/Graphics/Font.hpp
    inc/Graphics/GamePad.hpp
    inc/Graphics/GamePadState.hpp
    inc/Graphics/GamePadStateTracker.hpp
    inc/Graphics/Image.hpp
    inc/Graphics/Input.hpp
    inc/Graphics/Keyboard.hpp
    inc/Graphics/KeyboardState.hpp
    inc/Graphics/KeyboardStateTracker.hpp
    inc/Graphics/KeyCodes.hpp
    inc/Graphics/Material.hpp
    inc/Graphics/Mesh.hpp
    inc/Graphics/Model.hpp
    inc/Graphics/Mouse.hpp
    inc/Graphics/MouseState.hpp
    inc/Graphics/MouseStateTracker.hpp
    inc/Graphics/ResourceManager.hpp
    inc/Graphics/Sprite.hpp
    inc/Graphics/SpriteAnim.hpp
    inc/Graphics/SpriteSheet.hpp
    inc/Graphics/TileMap.hpp
    inc/Graphics/Timer.hpp
    inc/Graphics/Vertex.hpp
    inc/Graphics/Window.hpp
    inc/Graphics/WindowHandle.hpp
    inc/Graphics/WindowImpl.hpp
    inc/aligned_unique_ptr.hpp
    inc/hash.hpp
    inc/stb_easy_font.h
    inc/stb_image.h
    inc/stb_image_write.h
    inc/stb_truetype.h
)

set( SRC_FILES
    src/BlendMode.cpp
    src/Color.cpp
    src/Font.cpp
    src/FragmentShader.glsl
    src/GamePad.cpp
    src/GamePadStateTracker.cpp
    src/Image.cpp
    src/Input.cpp
    src/Keyboard.cpp
    src/KeyboardState.cpp
    src/KeyboardStateTracker.cpp
    src/Material.cpp
    src/Mesh.cpp
    src/Model.cpp
    src/Mouse.cpp
    src/ResourceManager.cpp
    src/SpriteAnim.cpp
    src/SpriteSheet.cpp
    src/VertexShader.glsl
    src/stb_image.cpp
    src/stb_image_write.cpp
    src/stb_rect_pack.h
    src/stb_truetype.cpp
    src/TileMap.cpp
    src/Timer.cpp
    src/Window.cpp
)

if(TARGET glfw)
    list( APPEND SRC_FILES
        src/GLFW/GamePadGLFW.cpp
        src/GLFW/KeyboardGLFW.cpp
        src/GLFW/MouseGLFW.cpp
        src/GLFW/WindowGLFW.cpp
        src/GLFW/WindowGLFW.hpp
    )
elseif(WIN32)
    list( APPEND SRC_FILES
        src/Win32/GamePadXInput.cpp
        src/Win32/IncludeWin32.hpp
        src/Win32/KeyboardWin32.cpp
        src/Win32/MouseWin32.cpp
        src/Win32/WindowWin32.hpp
        src/Win32/WindowWin32.cpp
    )
endif(TARGET glfw)

set( ALL_FILES 
    ${SRC_FILES} 
    ${INC_FILES} 
    ../.clang-format
)

add_library( Graphics ${ALL_FILES} )

set_target_properties( Graphics
    PROPERTIES
        CXX_STANDARD 20
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions( Graphics
        PRIVATE SoftwareRasterizer_EXPORTS
        INTERFACE SoftwareRasterizer_IMPORTS
    )
endif(BUILD_SHARED_LIBS)

target_include_directories( Graphics
    PUBLIC inc
)

target_link_libraries( Graphics
    PUBLIC Math fmt::fmt imgui
    PRIVATE glad tinyobjloader
)

if(TARGET glfw)
    target_link_libraries( Graphics
        PRIVATE glfw
    )
    target_compile_definitions( Graphics
        PRIVATE GLFW
    )
endif(TARGET glfw)

if(SR_USE_OPENMP)
    find_package(OpenMP)
    if(OpenMP_CXX_FOUND)
        target_include_directories( Graphics
            PRIVATE ${OpenMP_CXX_INCLUDE_DIRS}
        )

        target_link_libraries( Graphics
            PUBLIC OpenMP::OpenMP_CXX
        )
    endif(OpenMP_CXX_FOUND)
endif(SR_USE_OPENMP)

install(TARGETS Graphics)
install( DIRECTORY inc/Graphics DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )
