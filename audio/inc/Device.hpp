#pragma once

#include "Config.hpp"

#include <memory>

namespace Audio
{

class DeviceImpl;

class AUDIO_API Device
{
public:


    // Singleton class.
    Device() = delete;
    ~Device() = delete;
    Device( const Device& ) = delete;
    Device( Device&& )      = delete;
    Device& operator=( const Device& ) = delete;
    Device& operator=(Device&&) = delete;

private:
    static std::unique_ptr<DeviceImpl> instance;
};
}  // namespace Audio