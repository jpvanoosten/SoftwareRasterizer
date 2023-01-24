#include <Device.hpp>

#include <iostream>

#include "miniaudio.h"

namespace Audio
{
class DeviceImpl
{
public:
    DeviceImpl();
    ~DeviceImpl();

private:
    ma_engine engine;
};

}  // namespace Audio

using namespace Audio;

DeviceImpl::DeviceImpl()
{
    if ( ma_engine_init(nullptr, &engine) != MA_SUCCESS )
    {
        std::cerr << "Failed to initialize audio engine." << std::endl;
        return;
    }
}

DeviceImpl::~DeviceImpl()
{
    ma_engine_uninit( &engine );
}

std::unique_ptr<DeviceImpl> Device::instance = std::make_unique<DeviceImpl>();