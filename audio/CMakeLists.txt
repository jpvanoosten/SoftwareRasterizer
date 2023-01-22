cmake_minimum_required( VERSION 3.23.0 )

set( INC_FILES
    inc/Config.hpp
    inc/Device.hpp
    inc/Decoder.hpp
    inc/DecoderWAV.hpp
)

set( SRC_FILES
    src/DecoderWAV.cpp
    src/Device.cpp
    src/dr_flac.c
    src/dr_flac.h
    src/dr_mp3.c
    src/dr_mp3.h
    src/dr_wav.c
    src/dr_wav.h
#    src/miniaudio.c
#    src/miniaudio.h
#    src/minimp3.c
#    src/minimp3.h
#    src/minimp3_ext.h
    src/stb_vorbis.c
)

set( ALL_FILES 
    ${SRC_FILES} 
    ${INC_FILES} 
    ../.clang-format
)

add_library( Audio ${ALL_FILES} )

set_target_properties( Audio
    PROPERTIES
        CXX_STANDARD 20
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions( Audio
        PRIVATE Audio_EXPORTS
        INTERFACE Audio_IMPORTS
    )
endif(BUILD_SHARED_LIBS)

target_include_directories( Audio
    PUBLIC inc
)

target_link_libraries( Audio
    PUBLIC OpenAL
)
