cmake_minimum_required( VERSION 3.23.0 )

add_subdirectory(01-ClearScreen)
add_subdirectory(02-Triangle)
add_subdirectory(03-Sprites)
add_subdirectory(04-BlendModes)
add_subdirectory(05-CopyImage)
add_subdirectory(06-Fonts)

set_target_properties( 
	01-ClearScreen 
	02-Triangle
	03-Sprites
	04-BlendModes
	05-CopyImage
	06-Fonts
	PROPERTIES
		FOLDER samples
)
