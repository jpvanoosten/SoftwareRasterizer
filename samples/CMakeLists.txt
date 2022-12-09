cmake_minimum_required( VERSION 3.23.0 )

add_subdirectory(01-ClearScreen)
add_subdirectory(02-Triangle)
add_subdirectory(03-Sprites)
add_subdirectory(04-BlendModes)

set_target_properties( 
	01-ClearScreen 
	02-Triangle
	03-Sprites
	04-BlendModes
	PROPERTIES
		FOLDER samples
)
