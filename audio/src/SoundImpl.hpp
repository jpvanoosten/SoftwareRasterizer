#pragma once

#include "Listener.hpp"
#include "Sound.hpp"
#include "miniaudio.h"

#include <glm/vec3.hpp>

#include <filesystem>

namespace Audio
{
class SoundImpl
{
public:
    SoundImpl( const std::filesystem::path& filePath, ma_engine* pEngine, ma_sound_group* pGroup = nullptr, uint32_t flags = 0 );
    ~SoundImpl();

    void play();
    void stop();

    void setPinnedListener( const Listener& listener );

    void setVolume( float volume );
    float getVolume() const;

    void setPan( float pan );
    float getPan() const;

    void setPitch( float pitch );
    float getPitch() const;

    void setPosition( const glm::vec3& pos );
    glm::vec3 getPosition() const;

    void setDirection( const glm::vec3& dir );
    glm::vec3 getDirection() const;

    void setVelocity( const glm::vec3& vel );
    glm::vec3 getVelocity() const;

    void setCone( float innerConeAngle, float outerConeAngle, float outerGain );
    void getCone( float& innerConeAngle, float& outerConeAngle, float& outerGain );

    void setAttenuationModel( Sound::AttenuationModel attenuation );
    Sound::AttenuationModel getAttenuationModel() const;

protected:
private:
    ma_engine*      engine = nullptr;
    ma_sound_group* group  = nullptr;
    ma_sound        sound {};
};
}  // namespace Audio
