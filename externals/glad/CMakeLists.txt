cmake_minimum_required(VERSION 3.22.1) # The version of CMake used when creating this project.

set( INC_FILES
	include/glad/gl.h
	include/KHR/khrplatform.h
)

set( SRC_FILES
	src/gl.c
)

if(WIN32)
	list( APPEND INC_FILES
		include/glad/wgl.h
	)
	
    list( APPEND SRC_FILES
		src/wgl.c
	)
endif(WIN32)

set( ALL_FILES 
    ${SRC_FILES} 
    ${INC_FILES} 
)

add_library( glad STATIC ${ALL_FILES} )

target_include_directories( glad
    PUBLIC include
)