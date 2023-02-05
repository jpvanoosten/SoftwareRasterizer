#pragma once

#include "Config.hpp"

#include <memory>

namespace Audio
{
class SoundImpl;

class AUDIO_API Sound
{
public:
    
    Sound();
    ~Sound();
    Sound( const Sound& );
    Sound( Sound&& ) noexcept;
    Sound& operator=( const Sound& );
    Sound& operator=( Sound&& ) noexcept;

    /// <summary>
    /// Allow nullptr assignment.
    /// </summary>
    /// <remarks>
    /// Assigning `nullptr` will release the underlying implementation.
    /// This is the same as using the `reset` function on this object.
    /// </remarks>
    Sound& operator=( nullptr_t ) noexcept;

    /// <summary>
    /// Allow for null checks.
    /// </summary>
    bool operator==( nullptr_t ) const noexcept;
    bool operator!=( nullptr_t ) const noexcept;

    /// <summary>
    /// Explicit bool conversion allows to check for a valid object.
    /// </summary>
    /// <returns>`true` if this object contains a valid pointer to implementation. `false` otherwise.</returns>
    explicit operator bool() const noexcept;

protected:
    explicit Sound( std::shared_ptr<SoundImpl> impl );

private:
    std::shared_ptr<SoundImpl> impl;
};
}  // namespace Audio
