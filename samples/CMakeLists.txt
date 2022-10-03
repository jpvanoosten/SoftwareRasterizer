cmake_minimum_required( VERSION 3.23.0 )

add_subdirectory(01-ClearScreen)

set_target_properties( 
	01-ClearScreen
	PROPERTIES
		FOLDER samples
)

# Set the startup project.
set_directory_properties( PROPERTIES
    VS_STARTUP_PROJECT 01-ClearScreen
)
