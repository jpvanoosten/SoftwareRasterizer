#include <Graphics/Mouse.hpp>
#include <Graphics/Window.hpp>

#include "IncludeWin32.hpp"

#include <hidusage.h>

#include <mutex>

using namespace Graphics;

static MouseState g_globalState {};
static std::mutex g_stateMutex;

static HWND       g_hWnd { nullptr };
static std::mutex g_hWndMutex;

// Local mouse state is updated in ProcessEvents.
thread_local MouseState localState {};
thread_local int        relativeX { INT32_MAX };
thread_local int        relativeY { INT32_MAX };
thread_local bool       inFocus { true };

bool Mouse::isConnected()
{
    return GetSystemMetrics( SM_MOUSEPRESENT ) != 0;
}

bool Mouse::isVisible()
{
    CURSORINFO ci {};
    ci.cbSize = sizeof( CURSORINFO );
    if ( GetCursorInfo( &ci ) == FALSE )
    {
        return false;
    }

    return ( ci.flags & CURSOR_SHOWING ) != 0;
}

void Mouse::setVisible( bool visible )
{
    CURSORINFO ci {};
    ci.cbSize = sizeof( CURSORINFO );
    if ( GetCursorInfo( &ci ) == FALSE )
    {
        throw std::system_error( std::error_code( static_cast<int>( GetLastError() ), std::system_category() ),
                                 "GetCursorInfo" );
    }

    const bool isVisible = ( ci.flags & CURSOR_SHOWING ) != 0;
    if ( isVisible != visible )
    {
        ShowCursor( visible );
    }
}

MouseState Mouse::getState()
{
    std::lock_guard lock( g_stateMutex );

    const MouseState state = g_globalState;

    std::lock_guard lock2( g_hWndMutex );
    if ( g_hWnd )  // If the mouse is locked to a window, reset the x and y position of
                   // the mouse.
    {
        g_globalState.x = 0;
        g_globalState.y = 0;
    }

    return state;
}

static void ClipToWindow( HWND hWnd )
{
    if ( hWnd )
    {
        RECT rect;
        GetClientRect( hWnd, &rect );

        POINT tl { rect.left, rect.top };
        POINT br { rect.right, rect.bottom };

        // Convert to screen coordinates.
        MapWindowPoints( hWnd, nullptr, &tl, 1 );
        MapWindowPoints( hWnd, nullptr, &br, 1 );

        rect.left   = tl.x;
        rect.top    = tl.y;
        rect.right  = br.x;
        rect.bottom = br.y;

        ClipCursor( &rect );
    }
    else
    {
        ClipCursor( nullptr );
    }
}

void Mouse::lockToWindow( const Window& window )
{
    const HWND hWnd = window.getWindowHandle();

    std::lock_guard lock( g_hWndMutex );

    if ( g_hWnd == hWnd )
        return;

    if ( hWnd )
    {
        RAWINPUTDEVICE rid {};
        rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
        rid.usUsage     = HID_USAGE_GENERIC_MOUSE;
        rid.dwFlags     = RIDEV_INPUTSINK;
        rid.hwndTarget  = hWnd;
        if ( !RegisterRawInputDevices( &rid, 1, sizeof( RAWINPUTDEVICE ) ) )
        {
            throw std::system_error( std::error_code( static_cast<int>( GetLastError() ), std::system_category() ),
                                     "RegisterRawInputDevices" );
        }

        ClipToWindow( hWnd );

        g_hWnd = hWnd;
    }
}

void Mouse::unlock()
{
    std::lock_guard lock( g_hWndMutex );

    if ( g_hWnd )
    {
        g_hWnd = nullptr;

        RAWINPUTDEVICE rid {};
        rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
        rid.usUsage     = HID_USAGE_GENERIC_MOUSE;
        rid.dwFlags     = RIDEV_REMOVE;
        rid.hwndTarget  = nullptr;
        if ( !RegisterRawInputDevices( &rid, 1, sizeof( RAWINPUTDEVICE ) ) )
        {
            throw std::system_error( std::error_code( static_cast<int>( GetLastError() ), std::system_category() ),
                                     "RegisterRawInputDevices" );
        }

        ClipToWindow( nullptr );
    }
}

bool Mouse::isLocked()
{
    std::lock_guard lock( g_hWndMutex );
    return g_hWnd != nullptr;
}

glm::ivec2 Mouse::getPosition()
{
    POINT p;

    GetCursorPos( &p );

    return { p.x, p.y };
}

glm::ivec2 Mouse::getPosition( const Window& window )
{
    POINT p;
    GetCursorPos( &p );

    if ( const HWND hWnd = window.getWindowHandle() )
    {
        ScreenToClient( hWnd, &p );
    }

    return { p.x, p.y };
}

void Mouse::setPosition( const glm::ivec2& pos )
{
    SetCursorPos( pos.x, pos.y );
}

void Mouse::setPosition( const glm::ivec2& pos, const Window& window )
{
    if ( const HWND hWnd = window.getWindowHandle() )
    {
        POINT p { pos.x, pos.y };

        ClientToScreen( hWnd, &p );
        SetCursorPos( p.x, p.y );
    }
}

void CommitState( const MouseState& state )
{
    std::lock_guard lock( g_stateMutex );
    g_globalState = state;
}

void Mouse_ProcessMessage( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch ( message )
    {
    case WM_ACTIVATE:
    case WM_ACTIVATEAPP:
        if ( wParam )
        {
            inFocus = true;

            if ( g_hWnd )
            {
                // The mouse is being tracked relative to the window.
                localState.x = 0;
                localState.y = 0;
                relativeX    = INT32_MAX;
                relativeY    = INT32_MAX;

                ClipToWindow( g_hWnd );
            }
        }
        else
        {
            // Reset the mouse state (except for the scroll wheel values).
            const float vScrollWheel = localState.vScrollWheel;
            const float hScrollWheel = localState.hScrollWheel;

            memset( &localState, 0, sizeof( MouseState ) );

            localState.vScrollWheel = vScrollWheel;
            localState.hScrollWheel = hScrollWheel;

            ClipToWindow( nullptr );
            inFocus = false;
        }
        CommitState( localState );
        return;

    case WM_SIZE:
    {
        std::lock_guard lock( g_hWndMutex );
        // If the window is resized and the cursor is locked to the window, then update
        // the clip rectangle for the mouse cursor.
        ClipToWindow( g_hWnd );
    }
        return;

    case WM_INPUT:
    {
        std::lock_guard lock( g_hWndMutex );
        if ( inFocus && g_hWnd )
        {
            RAWINPUT raw {};
            UINT     rawSize = sizeof( RAWINPUT );

            const UINT resultData = GetRawInputData( reinterpret_cast<HRAWINPUT>( lParam ), RID_INPUT, &raw, &rawSize,
                                                     sizeof( RAWINPUTHEADER ) );
            if ( resultData == UINT( -1 ) )
            {
                throw std::runtime_error( "GetRawInputData" );
            }

            if ( raw.header.dwType == RIM_TYPEMOUSE )
            {
                const RAWMOUSE& mouseData = raw.data.mouse;
                if ( !( mouseData.usFlags & MOUSE_MOVE_ABSOLUTE ) )
                {
                    // When the cursor is locked to a window, the state's x, and y
                    // coordinates become relative movements.
                    // See:
                    // https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawmouse#remarks
                    localState.x = mouseData.lLastX;
                    localState.y = mouseData.lLastY;
                }
                else if ( mouseData.usFlags & MOUSE_VIRTUAL_DESKTOP )
                {
                    // This is used to make Remote Desktop sessions work
                    const int width  = GetSystemMetrics( SM_CXVIRTUALSCREEN );
                    const int height = GetSystemMetrics( SM_CYVIRTUALSCREEN );

                    auto const x = static_cast<int>( static_cast<float>( mouseData.lLastX ) / 65535.0f * static_cast<float>( width ) );
                    auto const y = static_cast<int>( static_cast<float>( mouseData.lLastY ) / 65535.0f * static_cast<float>( height ) );

                    if ( relativeX == INT32_MAX )
                    {
                        localState.x = 0;
                        localState.y = 0;
                    }
                    else
                    {
                        localState.x = x - relativeX;
                        localState.y = y - relativeY;
                    }

                    relativeX = x;
                    relativeY = y;
                }

                CommitState( localState );
            }
        }
    }
        return;

    case WM_MOUSEMOVE:
        break;

    case WM_LBUTTONDOWN:
        localState.leftButton = true;
        break;

    case WM_LBUTTONUP:
        localState.leftButton = false;
        break;

    case WM_RBUTTONDOWN:
        localState.rightButton = true;
        break;

    case WM_RBUTTONUP:
        localState.rightButton = false;
        break;

    case WM_MBUTTONDOWN:
        localState.middleButton = true;
        break;

    case WM_MBUTTONUP:
        localState.middleButton = false;
        break;

    case WM_MOUSEWHEEL:
        localState.vScrollWheel += GET_WHEEL_DELTA_WPARAM( wParam ) / static_cast<float>( WHEEL_DELTA );
        break;

    case WM_MOUSEHWHEEL:
        localState.hScrollWheel += GET_WHEEL_DELTA_WPARAM( wParam ) / static_cast<float>( WHEEL_DELTA );
        break;

    case WM_XBUTTONDOWN:
        switch ( GET_XBUTTON_WPARAM( wParam ) )
        {
        case XBUTTON1:
            localState.xButton1 = true;
            break;

        case XBUTTON2:
            localState.xButton2 = true;
            break;
        }
        break;

    case WM_XBUTTONUP:
        switch ( GET_XBUTTON_WPARAM( wParam ) )
        {
        case XBUTTON1:
            localState.xButton1 = false;
            break;

        case XBUTTON2:
            localState.xButton2 = false;
            break;
        }
        break;

    case WM_MOUSEHOVER:
        break;

    default:
        // Not a mouse message, so exit
        return;
    }

    // All mouse messages provide a new pointer position
    {
        std::lock_guard lock( g_hWndMutex );
        if ( !g_hWnd )  // If the mouse mode is absolute.
        {
            POINT p { GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) };

            // Mouse wheel events are screen relative.
            switch ( message )
            {
            case WM_MOUSEWHEEL:
            case WM_MOUSEHWHEEL:
                localState.screenX = p.x;
                localState.screenY = p.y;

                ::ScreenToClient( hWnd, &p );

                localState.x = p.x;
                localState.y = p.y;
                break;
            default:
                localState.x = p.x;
                localState.y = p.y;

                ::ClientToScreen( hWnd, &p );

                localState.screenX = p.x;
                localState.screenY = p.y;
            }
        }
    }

    CommitState( localState );
}
