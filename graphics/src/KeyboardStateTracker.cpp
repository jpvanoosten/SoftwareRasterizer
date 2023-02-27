#include <Graphics/KeyboardStateTracker.hpp>

#include <cstring> // For memset.

using namespace Graphics;


KeyboardStateTracker::KeyboardStateTracker() noexcept    // NOLINT(cppcoreguidelines-pro-type-member-init)
{
    reset();
}

bool KeyboardStateTracker::update( const KeyboardState& state ) noexcept
{
    auto currPtr     = reinterpret_cast<const uint32_t*>( &state );
    auto prevPtr     = reinterpret_cast<const uint32_t*>( &lastState );
    auto releasedPtr = reinterpret_cast<uint32_t*>( &released );
    auto pressedPtr  = reinterpret_cast<uint32_t*>( &pressed );

    for ( size_t j = 0; j < ( 256 / 32 ); ++j )
    {
        *pressedPtr  = *currPtr & ~( *prevPtr );
        *releasedPtr = ~( *currPtr ) & *prevPtr;

        ++currPtr;
        ++prevPtr;
        ++releasedPtr;
        ++pressedPtr;
    }

    if ( lastState != state )
    {
        lastState = state;
        return true;
    }

    return false;
}

void KeyboardStateTracker::reset() noexcept
{
    memset( this, 0, sizeof( KeyboardStateTracker ) );
}

bool KeyboardStateTracker::isKeyPressed( KeyCode key ) const noexcept
{
    return pressed.isKeyDown( key );
}

bool KeyboardStateTracker::isKeyReleased( KeyCode key ) const noexcept
{
    return released.isKeyDown( key );
}

KeyboardState KeyboardStateTracker::getLastState() const noexcept
{
    return lastState;
}