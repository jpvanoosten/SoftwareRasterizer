#pragma once

#include "miniaudio.h"

#include <filesystem>

namespace Audio
{
class SoundImpl
{
public:
    SoundImpl( const std::filesystem::path& filePath, ma_engine* pEngine, ma_sound_group* pGroup = nullptr, uint32_t flags = 0 );
    ~SoundImpl();

protected:
private:
    ma_engine*      engine = nullptr;
    ma_sound_group* group  = nullptr;
    ma_sound        sound {};
};
}  // namespace Audio
