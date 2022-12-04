cmake_minimum_required( VERSION 3.23.0 )

add_subdirectory(01-ClearScreen)
add_subdirectory(02-Triangle)
add_subdirectory(03-Sprites)

set_target_properties( 
	01-ClearScreen 
	02-Triangle
	03-Sprites
	PROPERTIES
		FOLDER samples
)
