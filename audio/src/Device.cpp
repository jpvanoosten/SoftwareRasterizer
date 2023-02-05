#include "ListenerImpl.hpp"

#include <Device.hpp>

#include <iostream>

#include "miniaudio.h"

namespace Audio
{
struct MakeListener : Listener
{
    MakeListener( std::shared_ptr<ListenerImpl> impl )
    : Listener( std::move( impl ) )
    {}
};

class DeviceImpl
{
public:
    DeviceImpl();
    ~DeviceImpl();

    Listener getListener( uint32_t listenerIndex );
    void      setMasterVolume( float volume );

private:
    ma_engine engine {};
};
}  // namespace Audio

using namespace Audio;

DeviceImpl::DeviceImpl()
{
    ma_engine_config config = ma_engine_config_init();
    config.listenerCount    = MA_ENGINE_MAX_LISTENERS;

    if ( ma_engine_init( &config, &engine ) != MA_SUCCESS )
    {
        std::cerr << "Failed to initialize audio engine." << std::endl;
        return;
    }
}

DeviceImpl::~DeviceImpl()
{
    ma_engine_uninit( &engine );
}

Listener DeviceImpl::getListener( uint32_t listenerIndex )
{
    if ( listenerIndex < MA_ENGINE_MAX_LISTENERS )
    {
        return MakeListener( std::make_shared<ListenerImpl>( listenerIndex, &engine ) );
    }

    return MakeListener( nullptr );
}

void DeviceImpl::setMasterVolume( float volume )
{
    ma_engine_set_volume( &engine, volume );
}

std::unique_ptr<DeviceImpl> Device::instance = std::make_unique<DeviceImpl>();

void Device::setMasterVolume( float volume )
{
    instance->setMasterVolume( volume );
}

Listener Device::getListener( uint32_t listenerIndex )
{
    return instance->getListener( listenerIndex );
}
