#include <Graphics/Events.hpp>
#include <Graphics/Keyboard.hpp>

using namespace Graphics;

#include <mutex>

using namespace Graphics;

static_assert( sizeof( KeyboardState ) == 256 / 8 );

// Global keyboard state.
static KeyboardState state {};
// Mutex to protect shared access to keyboard state.
static std::mutex stateMutex;

KeyboardState Keyboard::getState()
{
    std::lock_guard lock( stateMutex );

    state.ShiftKey   = state.LeftShift || state.RightShift;
    state.ControlKey = state.LeftControl || state.RightControl;
    state.AltKey     = state.LeftAlt || state.RightAlt;

    return state;
}

void Keyboard::reset()
{
    std::lock_guard lock( stateMutex );

    memset( &state, 0, sizeof( KeyboardState ) );
}

static void keyDown( int key ) noexcept
{
    if ( key < 0 || key > 0xfe )
        return;

    std::lock_guard lock { stateMutex };

    const auto ptr = reinterpret_cast<uint32_t*>( &state );

    const unsigned int bf = 1u << ( key & 0x1f );
    ptr[( key >> 5 )] |= bf;
}

static void keyUp( int key ) noexcept
{
    if ( key < 0 || key > 0xfe )
        return;

    std::lock_guard lock { stateMutex };

    const auto ptr = reinterpret_cast<uint32_t*>( &state );

    const unsigned int bf = 1u << ( key & 0x1f );
    ptr[( key >> 5 )] &= ~bf;
}

void Keyboard_ProcessMessage( const KeyEventArgs& e )
{
    if ( e.state == KeyState::Pressed )
    {
        keyDown( static_cast<int>( e.code ) );
    }
    else
    {
        keyUp( static_cast<int>( e.code ) );
    }
}