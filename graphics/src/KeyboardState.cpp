#include <Graphics/KeyboardState.hpp>

using namespace Graphics;

// Keyboard state is exactly 32 bytes.
static_assert( sizeof( KeyboardState ) == ( 256 / 8 ) );

bool KeyboardState::isKeyDown( KeyCode key ) const noexcept
{
    auto k = static_cast<uint8_t>( key );
    if ( k <= 0xfe )
    {
        const auto         ptr = reinterpret_cast<const uint32_t*>( this );
        const unsigned int bf  = 1u << ( k & 0x1f );
        return ( ptr[( k >> 5 )] & bf ) != 0;
    }
    return false;
}

bool KeyboardState::isKeyUp( KeyCode key ) const noexcept
{
    auto k = static_cast<uint8_t>( key );
    if ( k <= 0xfe )
    {
        const auto         ptr = reinterpret_cast<const uint32_t*>( this );
        const unsigned int bf  = 1u << ( k & 0x1f );
        return ( ptr[( k >> 5 )] & bf ) == 0;
    }
    return false;
}