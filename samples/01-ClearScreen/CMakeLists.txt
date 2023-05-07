cmake_minimum_required( VERSION 3.22.1 )

set( TARGET_NAME 01-ClearScreen )

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
    PUBLIC Graphics
)