cmake_minimum_required( VERSION 3.23.0 )

add_subdirectory(01-ClearScreen)
add_subdirectory(02-Triangle)

set_target_properties( 
	01-ClearScreen 
	02-Triangle
	PROPERTIES
		FOLDER samples
)
