#include <Device.hpp>

#include <al.h>
#include <alc.h>

#include <iostream>

namespace Audio
{
class DeviceImpl
{
public:
    DeviceImpl();
    ~DeviceImpl();

private:
    ALCdevice*  m_pDevice  = nullptr;
    ALCcontext* m_pContext = nullptr;
};

}  // namespace Audio

using namespace Audio;

DeviceImpl::DeviceImpl()
{
    // Create the audio device.
    m_pDevice = alcOpenDevice( nullptr );
    if ( m_pDevice )
    {
        m_pContext = alcCreateContext( m_pDevice, nullptr );

        if ( m_pContext )
        {
            alcMakeContextCurrent( m_pContext );
        }
        else
        {
            std::cerr << "An error occurred while creating the OpenAL context." << std::endl;
        }
    }
    else
    {
        std::cerr << "An error occurred while creating the OpenAL device." << std::endl;
    }
}

DeviceImpl::~DeviceImpl()
{
    alcMakeContextCurrent( nullptr );
    if ( m_pContext )
    {
        alcDestroyContext( m_pContext );
        m_pContext = nullptr;
    }

    if ( m_pDevice )
    {
        alcCloseDevice( m_pDevice );
        m_pDevice = nullptr;
    }
}

std::unique_ptr<DeviceImpl> Device::instance = std::make_unique<DeviceImpl>();