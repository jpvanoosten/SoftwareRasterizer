#pragma once

#include "Config.hpp"
#include "Listener.hpp"

#include <glm/vec3.hpp>

#include <memory>

namespace Audio
{

class DeviceImpl;

class AUDIO_API Device
{
public:
    /// <summary>
    /// Set the master volume for the audio device. A value of 0 is silent,
    /// a value of 1 is 100% volume and a value over 1 is amplification.
    /// </summary>
    /// <param name="volume">The master volume.</param>
    static void setMasterVolume( float volume );

    /// <summary>
    /// Get a audio listener at a particular index.
    /// </summary>
    /// <remarks>
    /// There is a maximum of 4 listeners at index 0 - 3. Requesting an out-of-range listener
    /// will result in an invalid listener.
    /// </remarks>
    /// <param name="listenerIndex">(optional) The listener index to retrieve. Default: 0</param>
    /// <returns>The listener at the specified index.</returns>
    static Listener getListener( uint32_t listenerIndex = 0 );

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