cmake_minimum_required(VERSION 3.22.1)

set( TARGET_NAME 00-Common )

set( INC
	CameraController.hpp
)

set( SRC
	CameraController.cpp
)

add_library( ${TARGET_NAME} STATIC ${INC} ${SRC} )

target_compile_features( ${TARGET_NAME} PUBLIC cxx_std_20 )

target_include_directories( ${TARGET_NAME}
	PUBLIC ./
)

target_link_libraries( ${TARGET_NAME}
	PUBLIC Graphics
)


