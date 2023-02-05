#include "SoundImpl.hpp"

#include <iostream>

using namespace Audio;

SoundImpl::SoundImpl( const std::filesystem::path& filePath, ma_engine* pEngine, ma_sound_group* pGroup, uint32_t flags )
: engine { pEngine }
, group { pGroup }
{

    if( ma_sound_init_from_file_w( engine, filePath.c_str(), flags, group, nullptr, &sound ) != MA_SUCCESS )
    {
        std::cerr << "Failed to initialize sound from source: " << filePath.string()  << std::endl;
    }
}

SoundImpl::~SoundImpl()
{
    ma_sound_uninit( &sound );
}
