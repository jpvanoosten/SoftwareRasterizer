cmake_minimum_required( VERSION 3.22.1 )

set( TARGET_NAME 09-Arkanoid )

set( INC_FILES
    inc/Ball.hpp
    inc/Brick.hpp
    inc/Button.hpp
    inc/Bullet.hpp
    inc/Game.hpp
    inc/GameOverState.hpp
    inc/HighScores.hpp
    inc/HighScoreState.hpp
    inc/Field.hpp
    inc/Level.hpp
    inc/ParseRects.hpp
    inc/Physics.hpp
    inc/PlayState.hpp
    inc/State.hpp
    inc/TitleState.hpp
    inc/Vaus.hpp
)

set( SRC_FILES
    main.cpp
    src/Ball.cpp
    src/Brick.cpp
    src/Button.cpp
    src/Bullet.cpp
    src/Game.cpp
    src/GameOverState.cpp
    src/HighScores.cpp
    src/HighScoreState.cpp
    src/Field.cpp
    src/Level.cpp
    src/ParseRects.cpp
    src/Stages.hpp
    src/Physics.cpp
    src/PlayState.cpp
    src/tinyxml2.h
    src/tinyxml2.cpp
    src/TitleState.cpp
    src/Vaus.cpp
)

set( ALL_FILES ${SRC_FILES} ${INC_FILES} ../glm.natvis README.md )

add_executable( ${TARGET_NAME} ${ALL_FILES})

set_target_properties( ${TARGET_NAME}
    PROPERTIES
        CXX_STANDARD 20
)

target_include_directories( ${TARGET_NAME}
    PUBLIC inc
)

target_link_libraries( ${TARGET_NAME} 
    PUBLIC Graphics
)

# Set Local Debugger Settings (Command Arguments and Environment Variables)
set( COMMAND_ARGUMENTS "-cwd \"${CMAKE_CURRENT_SOURCE_DIR}/..\"" )
configure_file( DebugSettings.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.vcxproj.user @ONLY )
