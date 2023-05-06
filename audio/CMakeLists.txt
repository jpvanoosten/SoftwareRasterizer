cmake_minimum_required( VERSION 3.23.0 )

include(FetchContent)

set( AUDIO_BUILD_EXAMPLES OFF )

FetchContent_Declare(
  Audio
  GIT_REPOSITORY https://github.com/jpvanoosten/Audio.git
  GIT_TAG        v0.0.1
)

message( STATUS "Fetching Audio library..." )
FetchContent_MakeAvailable(Audio)
