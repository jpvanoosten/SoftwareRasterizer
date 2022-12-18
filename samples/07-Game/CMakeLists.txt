cmake_minimum_required( VERSION 3.23.0 )

set( TARGET_NAME 07-Game )

set( INC_FILES
    inc/Background.hpp
    inc/Game.hpp
)

set( SRC_FILES
    main.cpp
    src/Background.cpp
    src/Game.cpp
)

set( ALL_FILES ${SRC_FILES} ${INC_FILES} )

add_executable( ${TARGET_NAME} ${ALL_FILES})

set_target_properties( ${TARGET_NAME}
    PROPERTIES
        CXX_STANDARD 20
)

target_link_libraries( ${TARGET_NAME} 
    PUBLIC SoftwareRasterizer LDtkLoader
)

target_include_directories( ${TARGET_NAME}
    PUBLIC inc
)

# Set Local Debugger Settings (Command Arguments and Environment Variables)
set( COMMAND_ARGUMENTS "-cwd \"${CMAKE_CURRENT_SOURCE_DIR}/..\"" )
configure_file( DebugSettings.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.vcxproj.user @ONLY )
