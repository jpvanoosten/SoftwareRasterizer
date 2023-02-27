#include <Graphics/Keyboard.hpp>

#include "IncludeWin32.hpp"

#include <mutex>

using namespace Graphics;

static_assert( sizeof( KeyboardState ) == 256 / 8 );

// Global keyboard state.
static KeyboardState state {};
// Mutex to protect shared access to keyboard state.
static std::mutex    stateMutex;

KeyboardState Keyboard::getState()
{
    std::lock_guard lock( stateMutex );

    state.ShiftKey = state.LeftShift || state.RightShift;
    state.ControlKey = state.LeftControl || state.RightControl;
    state.AltKey = state.LeftAlt || state.RightAlt;
    
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

void Keyboard_ProcessMessage( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    bool down = false;

    switch ( message )
    {
    case WM_ACTIVATE:
    case WM_ACTIVATEAPP:
        Keyboard::reset();
        return;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        down = true;
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        break;

    default:
        return;
    }

    int vk = static_cast<int>( wParam );
    switch ( vk )
    {
    case VK_SHIFT:
        vk = static_cast<int>( MapVirtualKeyW(
            ( static_cast<UINT>( lParam ) & 0x00ff0000 ) >> 16u, MAPVK_VSC_TO_VK_EX ) );
        if ( !down )
        {
            // Workaround to ensure left vs. right shift get cleared when both were
            // pressed at the same time.
            keyUp( VK_LSHIFT );
            keyUp( VK_RSHIFT );
        }
        break;

    case VK_CONTROL:
        vk = ( static_cast<UINT>( lParam ) & 0x01000000 ) ? VK_RCONTROL : VK_LCONTROL;
        break;

    case VK_MENU:
        vk = ( static_cast<UINT>( lParam ) & 0x01000000 ) ? VK_RMENU : VK_LMENU;
        break;
    }

    if ( down )
    {
        keyDown( vk );
    }
    else
    {
        keyUp( vk );
    }
}