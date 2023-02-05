#include <Sound.hpp>
#include "SoundImpl.hpp"

using namespace Audio;

Sound::Sound()                              = default;
Sound::~Sound()                             = default;
Sound::Sound( const Sound& )                = default;
Sound::Sound( Sound&& ) noexcept            = default;
Sound& Sound::operator=( const Sound& )     = default;
Sound& Sound::operator=( Sound&& ) noexcept = default;
Sound& Sound::operator=( nullptr_t ) noexcept
{
    impl = nullptr;
    return *this;
}

bool Sound::operator==( nullptr_t ) const noexcept
{
    return impl == nullptr;
}

bool Sound::operator!=( nullptr_t ) const noexcept
{
    return impl != nullptr;
}

Sound::operator bool() const noexcept
{
    return impl != nullptr;
}

Sound::Sound( std::shared_ptr<SoundImpl> impl )
: impl { std::move(impl) }
{}
