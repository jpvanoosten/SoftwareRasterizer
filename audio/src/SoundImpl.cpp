#include "SoundImpl.hpp"
#include "ListenerImpl.hpp"

#include <iostream>

using namespace Audio;

SoundImpl::SoundImpl( const std::filesystem::path& filePath, ma_engine* pEngine, ma_sound_group* pGroup, uint32_t flags )
: engine { pEngine }
, group { pGroup }
{

    if ( ma_sound_init_from_file_w( engine, filePath.c_str(), flags, group, nullptr, &sound ) != MA_SUCCESS )
    {
        std::cerr << "Failed to initialize sound from source: " << filePath.string() << std::endl;
    }
}

SoundImpl::~SoundImpl()
{
    ma_sound_uninit( &sound );
}

void SoundImpl::play()
{
    ma_sound_start( &sound );
}

void SoundImpl::stop()
{
    ma_sound_stop( &sound );
}

void SoundImpl::setPinnedListener( const Listener& listener )
{
    if ( const auto listenerImpl = listener.get() )
    {
        const uint32_t index = listenerImpl->getIndex();
        ma_sound_set_pinned_listener_index( &sound, index );
    }
}

void SoundImpl::setVolume( float volume )
{
    ma_sound_set_volume( &sound, volume );
}

float SoundImpl::getVolume() const
{
    return ma_sound_get_volume( &sound );
}

void SoundImpl::setPan( float pan )
{
    ma_sound_set_pan( &sound, pan );
}

float SoundImpl::getPan() const
{
    return ma_sound_get_pan( &sound );
}

void SoundImpl::setPitch( float pitch )
{
    ma_sound_set_pitch( &sound, pitch );
}

float SoundImpl::getPitch() const
{
    return ma_sound_get_pitch( &sound );
}

void SoundImpl::setPosition( const glm::vec3& pos )
{
    ma_sound_set_position( &sound, pos.x, pos.y, pos.z );
}

glm::vec3 SoundImpl::getPosition() const
{
    auto pos = ma_sound_get_position( &sound );
    return { pos.x, pos.y, pos.z };
}

void SoundImpl::setDirection( const glm::vec3& dir )
{
    ma_sound_set_direction( &sound, dir.x, dir.y, dir.z );
}

glm::vec3 SoundImpl::getDirection() const
{
    auto dir = ma_sound_get_direction( &sound );
    return { dir.x, dir.y, dir.z };
}

void SoundImpl::setVelocity( const glm::vec3& vel )
{
    ma_sound_set_velocity( &sound, vel.x, vel.y, vel.z );
}

glm::vec3 SoundImpl::getVelocity() const
{
    auto vel = ma_sound_get_velocity( &sound );
    return { vel.x, vel.y, vel.z };
}

void SoundImpl::setCone( float innerConeAngle, float outerConeAngle, float outerGain )
{
    ma_sound_set_cone( &sound, innerConeAngle, outerConeAngle, outerGain );
}

void SoundImpl::getCone( float& innerConeAngle, float& outerConeAngle, float& outerGain )
{
    ma_sound_get_cone( &sound, &innerConeAngle, &outerConeAngle, &outerGain );
}

void SoundImpl::setAttenuationModel( Sound::AttenuationModel attenuation )
{
    switch ( attenuation )
    {
    case Sound::AttenuationModel::None:
        ma_sound_set_attenuation_model( &sound, ma_attenuation_model_none );
        break;
    case Sound::AttenuationModel::Inverse:
        ma_sound_set_attenuation_model( &sound, ma_attenuation_model_inverse );
        break;
    case Sound::AttenuationModel::Linear:
        ma_sound_set_attenuation_model( &sound, ma_attenuation_model_linear );
        break;
    case Sound::AttenuationModel::Exponential:
        ma_sound_set_attenuation_model( &sound, ma_attenuation_model_exponential );
        break;
    }
}

Sound::AttenuationModel SoundImpl::getAttenuationModel() const
{
    switch ( ma_sound_get_attenuation_model( &sound ) )
    {
    case ma_attenuation_model_none:
        return Sound::AttenuationModel::None;
    case ma_attenuation_model_inverse:
        return Sound::AttenuationModel::Inverse;
    case ma_attenuation_model_linear:
        return Sound::AttenuationModel::Linear;
    case ma_attenuation_model_exponential:
        return Sound::AttenuationModel::Exponential;
    }

    return Sound::AttenuationModel::None;
}
