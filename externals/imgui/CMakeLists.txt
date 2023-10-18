cmake_minimum_required( VERSION 3.22.1 )

option( BUILD_SHARED_LIBS "Build ImGui as a shared library." OFF )
option( IMGUI_BACKEND_ALLEGRO5 "Add backend files for Allegro 5." OFF )
option( IMGUI_BACKEND_ANDROID "Add backend files for Android." OFF )
option( IMGUI_BACKEND_DX9 "Add backend files for DirectX 9." OFF )
option( IMGUI_BACKEND_DX10 "Add backend files for DirectX 10." OFF )
option( IMGUI_BACKEND_DX11 "Add backend files for DirectX 11." OFF )
option( IMGUI_BACKEND_DX12 "Add backend files for DirectX 12." OFF )
option( IMGUI_BACKEND_GLFW "Add backend files for GLFW." OFF )
option( IMGUI_BACKEND_GLUT "Add backend files for GLUT." OFF )
option( IMGUI_BACKEND_METAL "Add backend files for Metal." OFF )
option( IMGUI_BACKEND_OPENGL2 "Add backend files for OpenGL 2.0+" OFF )
option( IMGUI_BACKEND_OPENGL3 "Add backend files for OpenGL 3.0+" OFF )
option( IMGUI_BACKEND_OSX "Add backend files for OSX." OFF )
option( IMGUI_BACKEND_SDL2 "Add backend files for SDL2." OFF )
option( IMGUI_BACKEND_SDL3 "Add backend files for SDL3." OFF )
option( IMGUI_BACKEND_SDLRENDERER2 "Add backend files for SDL2/SDL_Renderer." OFF )
option( IMGUI_BACKEND_SDLRENDERER3 "Add backend files for SDL3/SDL_Renderer." OFF )
option( IMGUI_BACKEND_VULKAN "Add backend files for Vulkan." OFF )
option( IMGUI_BACKEND_WGPU "Add backend files for WebGPU." OFF )
option( IMGUI_BACKEND_WIN32 "Add backend files for Win32." OFF )
option( IMGUI_BUILD_EXAMPLES "Add examples for enabled backends." OFF )

set( IMGUI_VERSION_MAJOR 1 )
set( IMGUI_VERSION_MINOR 89 )
set( IMGUI_VERSION_PATCH 9 )
set( IMGUI_VERSION ${IMGUI_VERSION_MAJOR}.${IMGUI_VERSION_MINOR}.${IMGUI_VERSION_PATCH} )

# Use solution folder.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project( imgui VERSION ${IMGUI_VERSION} LANGUAGES C CXX )

set( IMGUI_INC
	imconfig.h
	imgui.h
	imgui_internal.h
	user_config.h
)

set( IMGUI_SRC
	imgui.cpp
	imgui_demo.cpp
	imgui_draw.cpp
	imgui_tables.cpp
	imgui_widgets.cpp
)

if( IMGUI_BACKEND_ALLEGRO5 )
	list( APPEND IMGUI_INC
		backends/imgui_impl_allegro5.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_allegro5.cpp
	)
endif( IMGUI_BACKEND_ALLEGRO5 )

if( IMGUI_BACKEND_ANDROID )
	list( APPEND IMGUI_INC 
		backends/imgui_impl_android.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_android.cpp
	)
endif( IMGUI_BACKEND_ANDROID )

if( IMGUI_BACKEND_DX9 )
	list( APPEND IMGUI_INC
		backends/imgui_impl_dx9.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_dx9.cpp
	)
endif( IMGUI_BACKEND_DX9 )

if( IMGUI_BACKEND_DX10 )
	list( APPEND IMGUI_INC
		backends/imgui_impl_dx10.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_dx10.cpp
	)
endif( IMGUI_BACKEND_DX10 )

if( IMGUI_BACKEND_DX11 )
	list( APPEND IMGUI_INC
		backends/imgui_impl_dx11.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_dx11.cpp
	)
endif( IMGUI_BACKEND_DX11 )

if( IMGUI_BACKEND_DX12 )
	list( APPEND IMGUI_INC
		backends/imgui_impl_dx12.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_dx12.cpp
	)
endif( IMGUI_BACKEND_DX12 )

if( IMGUI_BACKEND_GLFW )
	list( APPEND IMGUI_INC
		backends/imgui_impl_glfw.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_glfw.cpp
	)
endif( IMGUI_BACKEND_GLFW )

if( IMGUI_BACKEND_GLUT )
	list( APPEND IMGUI_INC
		backends/imgui_impl_glut.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_glut.cpp
	)
endif( IMGUI_BACKEND_GLUT )

if( IMGUI_BACKEND_METAL )
	list( APPEND IMGUI_INC
		backends/imgui_impl_metal.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_metal.mm
	)
endif( IMGUI_BACKEND_METAL )

if( IMGUI_BACKEND_OPENGL2 )
	list( APPEND IMGUI_INC
		backends/imgui_impl_opengl2.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_opengl2.cpp
	)
endif( IMGUI_BACKEND_OPENGL2 )

if( IMGUI_BACKEND_OPENGL3 )
	list( APPEND IMGUI_INC
		backends/imgui_impl_opengl3.h
		backends/imgui_impl_opengl3_loader.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_opengl3.cpp
	)
endif( IMGUI_BACKEND_OPENGL3 )

if( IMGUI_BACKEND_OSX )
	list( APPEND IMGUI_INC
		backends/imgui_impl_osx.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_osx.mm
	)
endif( IMGUI_BACKEND_OSX )

if( IMGUI_BACKEND_SDL2 )
	find_package( SDL 2...< REQUIRED )

	list( APPEND IMGUI_INC
		backends/imgui_impl_sdl2.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_sdl2.cpp
	)
endif( IMGUI_BACKEND_SDL2 )

if( IMGUI_BACKEND_SDL3 )
	find_package( SDL 3...< REQUIRED )

	list( APPEND IMGUI_INC
		backends/imgui_impl_sdl3.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_sdl3.cpp
	)
endif( IMGUI_BACKEND_SDL3 )

if( IMGUI_BACKEND_SDLRENDERER2 )
	list( APPEND IMGUI_INC
		backends/imgui_impl_sdlrenderer2.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_sdlrenderer2.cpp
	)
endif( IMGUI_BACKEND_SDLRENDERER2 )

if( IMGUI_BACKEND_SDLRENDERER3 )
	list( APPEND IMGUI_INC
		backends/imgui_impl_sdlrenderer3.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_sdlrenderer3.cpp
	)
endif( IMGUI_BACKEND_SDLRENDERER3 )

if( IMGUI_BACKEND_VULKAN )
	find_package( Vulkan REQUIRED COMPONENTS glslangValidator )

	add_custom_command( 
		OUTPUT backends/vulkan/glsl_shader.frag.u32
		COMMAND Vulkan::glslangValidator -V -x -o glsl_shader.frag.u32 glsl_shader.frag
		DEPENDS backends/vulkan/glsl_shader.frag
		WORKING_DIRECTORY backends/vulkan
	)
	add_custom_command( 
		OUTPUT backends/vulkan/glsl_shader.vert.u32
		COMMAND Vulkan::glslangValidator -V -x -o glsl_shader.vert.u32 glsl_shader.frag
		DEPENDS backends/vulkan/glsl_shader.vert
		WORKING_DIRECTORY backends/vulkan
	)

	list( APPEND IMGUI_INC
		backends/imgui_impl_vulkan.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_vulkan.cpp
		backends/vulkan/glsl_shader.vert
		backends/vulkan/glsl_shader.frag
		backends/vulkan/glsl_shader.vert.u32
		backends/vulkan/glsl_shader.frag.u32
	)
endif( IMGUI_BACKEND_VULKAN )

if( IMGUI_BACKEND_WGPU )
	list( APPEND IMGUI_INC
		backends/imgui_impl_wgpu.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_wgpu.cpp
	)
endif( IMGUI_BACKEND_WGPU )

if( IMGUI_BACKEND_WIN32 )
	list( APPEND IMGUI_INC
		backends/imgui_impl_win32.h
	)
	list( APPEND IMGUI_SRC
		backends/imgui_impl_win32.cpp
	)
endif( IMGUI_BACKEND_WIN32 )

add_library( imgui ${IMGUI_INC} ${IMGUI_SRC} )

target_include_directories( imgui
	PUBLIC . backends
)

target_compile_definitions( imgui
	PRIVATE IMGUI_USER_CONFIG="user_config.h"
)

if( BUILD_SHARED_LIBS )
	target_compile_definitions( imgui
		PRIVATE imgui_EXPORTS
		INTERFACE imgui_IMPORTS
	)
endif(BUILD_SHARED_LIBS)

if( IMGUI_BUILD_EXAMPLES )
	if( IMGUI_BACKEND_ALLEGRO5 )
		add_subdirectory( examples/example_allegro5 )
		set_target_properties( example_allegro5 PROPERTIES FOLDER examples )
	endif( IMGUI_BACKEND_ALLEGRO5 )

	if( IMGUI_BACKEND_ANDROID )
		# TODO: 
	endif( IMGUI_BACKEND_ANDROID )

	if( IMGUI_BACKEND_METAL )
		# TODO: 
	endif( IMGUI_BACKEND_METAL )

	if( IMGUI_BACKEND_WIN32 AND IMGUI_BACKEND_OPENGL3 )
		add_subdirectory( examples/example_win32_opengl3 )
		set_target_properties( example_win32_opengl3 PROPERTIES FOLDER examples )
	endif( IMGUI_BACKEND_WIN32 AND IMGUI_BACKEND_OPENGL3 )

endif( IMGUI_BUILD_EXAMPLES )