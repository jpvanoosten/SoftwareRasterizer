cmake_minimum_required( VERSION 3.22.1 )

set( TARGET_NAME 11-Rasterizer )

set( SRC_FILES
    main.cpp
)

set( INC_FILES

)

set( ALL_FILES ${SRC_FILES} ${INC_FILES} )

add_executable( ${TARGET_NAME} ${ALL_FILES})

set_target_properties( ${TARGET_NAME}
    PROPERTIES
        CXX_STANDARD 20
)

target_link_libraries( ${TARGET_NAME} 
    PUBLIC Graphics 00-Common
)

# Set Local Debugger Settings (Command Arguments and Environment Variables)
set( COMMAND_ARGUMENTS "-cwd \"${CMAKE_CURRENT_SOURCE_DIR}/..\"" )
configure_file( DebugSettings.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.vcxproj.user @ONLY )
