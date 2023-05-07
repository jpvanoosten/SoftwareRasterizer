cmake_minimum_required( VERSION 3.22.1 )

include(FetchContent)

set( AUDIO_BUILD_EXAMPLES OFF )

FetchContent_Declare(
  Audio
  GIT_REPOSITORY https://github.com/jpvanoosten/Audio.git
  GIT_TAG main
)

message( STATUS "Fetching Audio library..." )
FetchContent_MakeAvailable(Audio)
