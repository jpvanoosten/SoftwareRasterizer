#include "WindowWin32.hpp"

#include <comdef.h>
#include <objbase.h>

#include <iostream>

using namespace sr;

constexpr const wchar_t* WINDOW_CLASS_NAME = L"RasterizerWindow";

void ReportError()
{
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    DWORD  dw = ::GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
        (LPTSTR)&lpMsgBuf,
        0, NULL );

    // Display the error message and exit the process
    std::cerr << "Error: " << (char*)lpMsgBuf << std::endl;

    LocalFree( lpMsgBuf );
}

WindowWin32::WindowWin32( std::wstring_view title, int width, int height )
{
    static bool first = true;

    if ( first )
    {
        first = false;
        init();
    }

    RECT windowRect { 0, 0, width, height };
    AdjustWindowRect( &windowRect, WS_OVERLAPPEDWINDOW, FALSE );
    width  = windowRect.right - windowRect.left;
    height = windowRect.bottom - windowRect.top;

    m_hWnd = ::CreateWindowExW( 0, WINDOW_CLASS_NAME, title.data(), WS_OVERLAPPEDWINDOW, 0, 0, width, height, NULL, NULL, ::GetModuleHandleW( nullptr ), this );
}

WindowWin32::~WindowWin32()
{
    ::DestroyWindow( m_hWnd );
}

void WindowWin32::init()
{
    // Windows 10 Creators update adds Per Monitor V2 DPI awareness context.
    // Using this awareness context allows the client area of the window
    // to achieve 100% scaling while still allowing non-client window content to
    // be rendered in a DPI sensitive fashion.
    // @see
    // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setthreaddpiawarenesscontext
    SetProcessDpiAwarenessContext( DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 );

    // Initializes the COM library for use by the calling thread, sets the thread's
    // concurrency model, and creates a new
    // apartment for the thread if one is required.
    // This must be called at least once for each thread that uses the COM library.
    // @see
    // https://docs.microsoft.com/en-us/windows/win32/api/objbase/nf-objbase-coinitialize
    HRESULT hr = ::CoInitialize( nullptr );
    if ( FAILED( hr ) )
    {
        const _com_error err( hr );  // I hope this never happens.
        throw std::exception( err.ErrorMessage() );
    }

    WNDCLASSEXW wndClass {};

    wndClass.cbSize        = sizeof( WNDCLASSEX );
    wndClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndClass.lpfnWndProc   = &WndProc;
    wndClass.hInstance     = GetModuleHandleW( nullptr );
    wndClass.hCursor       = LoadCursor( nullptr, IDC_ARROW );
    wndClass.hIcon         = nullptr;
    wndClass.hbrBackground = reinterpret_cast<HBRUSH>( ( COLOR_WINDOW + 1 ) );
    wndClass.lpszMenuName  = nullptr;
    wndClass.lpszClassName = WINDOW_CLASS_NAME;
    wndClass.hIconSm       = nullptr;

    if ( !RegisterClassExW( &wndClass ) )
    {
        ::MessageBoxA( nullptr, "Unable to register the window class.", "Error", MB_OK | MB_ICONERROR );
    }
}

void WindowWin32::show()
{
    ::ShowWindow( m_hWnd, SW_SHOW );
}

void WindowWin32::present( const Image& image )
{
    // Invalidate the window's client rectangle so it gets repainted.
    RECT clientRect;
    ::GetClientRect( m_hWnd, &clientRect );
    ::InvalidateRect( m_hWnd, &clientRect, FALSE );

    PAINTSTRUCT ps;
    HDC         hdc = ::BeginPaint( m_hWnd, &ps );
    if ( hdc == nullptr )
        return;

    int srcWidth  = static_cast<int>( image.getWidth() );
    int srcHeight = static_cast<int>( image.getHeight() );
    int dstWidth  = clientRect.right - clientRect.left;
    int dstHeight = clientRect.bottom - clientRect.top;

    BITMAPINFO bmi {
        .bmiHeader {
            .biSize        = sizeof( BITMAPINFOHEADER ),
            .biWidth       = srcWidth,
            .biHeight      = -srcHeight,  // If biHeight is negative, the bitmap is a top-down DIB and its origin is the upper-left corner.
            .biPlanes      = 1,
            .biBitCount    = 32,
            .biCompression = BI_RGB,
        }
    };

    if ( srcWidth == dstWidth && srcHeight == dstHeight )
    {
        ::SetDIBitsToDevice( hdc, 0, 0, srcWidth, srcHeight, 0, 0, 0, srcHeight, image.data(), &bmi, DIB_RGB_COLORS );
    }
    else
    {
        ::SetStretchBltMode( hdc, HALFTONE );
        ::StretchDIBits( hdc, 0, 0, dstWidth, dstHeight, 0, 0, srcWidth, srcHeight, image.data(), &bmi, DIB_RGB_COLORS, SRCCOPY );
    }

    ::EndPaint( m_hWnd, &ps );
}

void WindowWin32::pushEvent( const Event& e )
{
    m_eventQueue.push( e );
}

void WindowWin32::onClose()
{
    const Event e { .type = Event::Close };
    pushEvent( e );
}

void WindowWin32::onKeyPressed( KeyEventArgs& args )
{
    const Event e {
        .type = Event::KeyPressed,
        .key  = args,
    };
    pushEvent( e );
}

void WindowWin32::onKeyReleased( KeyEventArgs& args )
{
    const Event e {
        .type = Event::KeyReleased,
        .key  = args,
    };
    pushEvent( e );
}

void WindowWin32::onResize( ResizeEventArgs& args )
{
    const Event e {
        .type   = Event::Resize,
        .resize = args
    };
    pushEvent( e );
}

void WindowWin32::onEndResize()
{
    RECT clientRect;
    ::GetClientRect( m_hWnd, &clientRect );

    const Event e {
        .type = Event::EndResize,
        .resize {
            .width  = clientRect.right - clientRect.left,
            .height = clientRect.bottom - clientRect.top }
    };

    pushEvent( e );
}

void WindowWin32::onMouseEnter( MouseMovedEventArgs& args )
{
    const Event e {
        .type      = Event::MouseEnter,
        .mouseMove = args
    };
    pushEvent( e );
}

void WindowWin32::onMouseButtonPressed( MouseButtonEventArgs& args )
{
    const Event e {
        .type        = Event::MouseButtonPressed,
        .mouseButton = args
    };
    pushEvent( e );
}

void WindowWin32::onMouseButtonReleased( MouseButtonEventArgs& args )
{
    const Event e {
        .type        = Event::MouseButtonReleased,
        .mouseButton = args
    };
    pushEvent( e );
}

void WindowWin32::onMouseWheel( MouseWheelEventArgs& args )
{
    const Event e {
        .type       = Event::MouseWheel,
        .mouseWheel = args
    };
    pushEvent( e );
}

void WindowWin32::onMouseHWheel( MouseWheelEventArgs& args )
{
    const Event e {
        .type       = Event::MouseHWheel,
        .mouseWheel = args
    };
    pushEvent( e );
}

void WindowWin32::onMouseLeave()
{
    const Event e {
        .type = Event::MouseLeave
    };
    pushEvent( e );
}

void WindowWin32::trackMouseEvents() const
{
    // Track mouse leave events.
    TRACKMOUSEEVENT trackMouseEvent {};
    trackMouseEvent.cbSize    = sizeof( TRACKMOUSEEVENT );
    trackMouseEvent.hwndTrack = m_hWnd;
    trackMouseEvent.dwFlags   = TME_LEAVE;
    TrackMouseEvent( &trackMouseEvent );
}

void WindowWin32::onMouseMoved( MouseMovedEventArgs& args )
{
    if ( !inClientRect )
    {
        previousMouseX = args.screenX;
        previousMouseY = args.screenY;

        inClientRect = true;

        trackMouseEvents();

        // Also invoke a mouse enter event.
        onMouseEnter( args );
    }

    args.relX = args.screenX - previousMouseX;
    args.relY = args.screenY - previousMouseY;

    previousMouseX = args.screenX;
    previousMouseY = args.screenY;

    const Event e {
        .type      = Event::MouseMoved,
        .mouseMove = args
    };
    pushEvent( e );
}

bool WindowWin32::popEvent( Event& event )
{
    if ( m_eventQueue.empty() )
    {
        processEvents();
    }

    if ( m_eventQueue.empty() )
        return false;

    event = m_eventQueue.front();
    m_eventQueue.pop();

    return true;
}

int WindowWin32::getWidth() const noexcept
{
    RECT rect;
    ::GetClientRect( m_hWnd, &rect );

    return rect.right - rect.left;
}

int WindowWin32::getHeight() const noexcept
{
    RECT rect;
    ::GetClientRect( m_hWnd, &rect );

    return rect.bottom - rect.top;
}

glm::ivec2 WindowWin32::getSize() const noexcept
{
    RECT rect;
    ::GetClientRect( m_hWnd, &rect );

    return { rect.right - rect.left, rect.bottom - rect.top };
}

void WindowWin32::processEvents()
{
    MSG msg;
    while ( ::PeekMessageW( &msg, m_hWnd, 0, 0, PM_REMOVE ) )
    {
        ::TranslateMessage( &msg );
        ::DispatchMessageW( &msg );
    }
}

static MouseButton DecodeMouseButton( UINT messageID )
{
    MouseButton mouseButton = MouseButton::None;

    switch ( messageID )
    {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_LBUTTONDBLCLK:
        mouseButton = MouseButton::Left;
        break;
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_RBUTTONDBLCLK:
        mouseButton = MouseButton::Right;
        break;
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MBUTTONDBLCLK:
        mouseButton = MouseButton::Middle;
        break;
    default:
        break;
    }

    return mouseButton;
}

// Convert wParam of the WM_SIZE events to a WindowState.
static WindowState DecodeWindowState( WPARAM wParam )
{
    WindowState windowState = WindowState::Restored;

    switch ( wParam )
    {
    case SIZE_RESTORED:
        windowState = WindowState::Restored;
        break;
    case SIZE_MINIMIZED:
        windowState = WindowState::Minimized;
        break;
    case SIZE_MAXIMIZED:
        windowState = WindowState::Maximized;
        break;
    default:
        break;
    }

    return windowState;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    if ( msg == WM_CREATE )
    {
        // Associate the Window pointer with the user data of when Win32 window.
        auto cs     = reinterpret_cast<CREATESTRUCT*>( lParam );
        auto window = reinterpret_cast<LONG_PTR>( cs->lpCreateParams );

        // Store the window pointer to the user data associated with the window handle.
        ::SetWindowLongPtrW( hWnd, GWLP_USERDATA, window );
    }

    auto window = reinterpret_cast<WindowWin32*>( ::GetWindowLongPtrW( hWnd, GWLP_USERDATA ) );

    if ( window )
    {
        switch ( msg )
        {
        case WM_CLOSE:
            window->onClose();
            break;
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        {
            MSG charMsg;

            // Get the Unicode character (UTF-16)
            unsigned int c = 0;
            // For printable characters, the next message will be WM_CHAR.
            // This message contains the character code we need to send the
            // KeyPressed event. Inspired by the SDL 1.2 implementation.
            if ( PeekMessage( &charMsg, hWnd, 0, 0, PM_NOREMOVE ) && charMsg.message == WM_CHAR )
            {
                c = static_cast<unsigned int>( charMsg.wParam );
            }

            KeyEventArgs e {
                .code      = static_cast<KeyCode>( wParam ),
                .character = c,
                .state     = KeyState::Pressed,
                .ctrl      = ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 ) != 0,
                .shift     = ( GetAsyncKeyState( VK_SHIFT ) & 0x8000 ) != 0,
                .alt       = ( GetAsyncKeyState( VK_MENU ) & 0x8000 ) != 0,
                .super     = ( GetAsyncKeyState( VK_APPS ) & 0x8000 ) != 0,
            };
            window->onKeyPressed( e );
        }
        break;
        case WM_SYSKEYUP:
        case WM_KEYUP:
        {
            unsigned int c        = 0;
            unsigned int scanCode = ( lParam & 0x00FF0000 ) >> 16;

            // Determine which key was released by converting the key code and
            // the scan code to a printable character (if possible). Inspired by
            // the SDL 1.2 implementation.
            unsigned char keyboardState[256];
            std::ignore = GetKeyboardState( keyboardState );

            wchar_t translatedCharacters[4];
            if ( int result = ToUnicodeEx( static_cast<UINT>( wParam ), scanCode, keyboardState, translatedCharacters,
                                           4, 0, nullptr ) > 0 )
            {
                c = translatedCharacters[0];
            }

            KeyEventArgs e {
                .code      = static_cast<KeyCode>( wParam ),
                .character = c,
                .state     = KeyState::Released,
                .ctrl      = ( GetAsyncKeyState( VK_CONTROL ) & 0x8000 ) != 0,
                .shift     = ( GetAsyncKeyState( VK_SHIFT ) & 0x8000 ) != 0,
                .alt       = ( GetAsyncKeyState( VK_MENU ) & 0x8000 ) != 0,
                .super     = ( GetAsyncKeyState( VK_APPS ) & 0x8000 ) != 0,
            };
            window->onKeyReleased( e );
        }
        break;
        // The default window procedure will play a system notification sound
        // when pressing the Alt+Enter keyboard combination if this message is
        // not handled.
        case WM_SYSCHAR:
            break;

        case WM_MOUSEMOVE:
        {
            int x = GET_X_LPARAM( lParam );
            int y = GET_Y_LPARAM( lParam );

            POINT p { x, y };
            ::ClientToScreen( hWnd, &p );

            MouseMovedEventArgs e {
                .leftButton   = ( wParam & MK_LBUTTON ) != 0,
                .middleButton = ( wParam & MK_MBUTTON ) != 0,
                .rightButton  = ( wParam & MK_RBUTTON ) != 0,
                .ctrl         = ( wParam & MK_CONTROL ) != 0,
                .shift        = ( wParam & MK_SHIFT ) != 0,
                .alt          = ( GetAsyncKeyState( VK_MENU ) & 0x8000 ) != 0,
                .super        = ( GetAsyncKeyState( VK_APPS ) & 0x8000 ) != 0,
                .x            = x,
                .y            = y,
                .screenX      = p.x,
                .screenY      = p.y
            };
            window->onMouseMoved( e );
        }
        break;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        {
            // Capture mouse movement until the button is released.
            ::SetCapture( hWnd );

            int x = GET_X_LPARAM( lParam );
            int y = GET_Y_LPARAM( lParam );

            POINT p { x, y };
            ::ClientToScreen( hWnd, &p );

            MouseButtonEventArgs e {
                .button       = DecodeMouseButton( msg ),
                .state        = ButtonState::Pressed,
                .leftButton   = ( wParam & MK_LBUTTON ) != 0,
                .middleButton = ( wParam & MK_MBUTTON ) != 0,
                .rightButton  = ( wParam & MK_RBUTTON ) != 0,
                .ctrl         = ( wParam & MK_CONTROL ) != 0,
                .shift        = ( wParam & MK_SHIFT ) != 0,
                .alt          = ( GetAsyncKeyState( VK_MENU ) & 0x8000 ) != 0,
                .super        = ( GetAsyncKeyState( VK_APPS ) & 0x8000 ) != 0,
                .x            = x,
                .y            = y,
                .screenX      = p.x,
                .screenY      = p.y
            };
            window->onMouseButtonPressed( e );
        }
        break;
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        {
            // Stop capturing the mouse.
            ::ReleaseCapture();

            int x = GET_X_LPARAM( lParam );
            int y = GET_Y_LPARAM( lParam );

            POINT p { x, y };
            ::ClientToScreen( hWnd, &p );

            MouseButtonEventArgs e {
                .button       = DecodeMouseButton( msg ),
                .state        = ButtonState::Released,
                .leftButton   = ( wParam & MK_LBUTTON ) != 0,
                .middleButton = ( wParam & MK_MBUTTON ) != 0,
                .rightButton  = ( wParam & MK_RBUTTON ) != 0,
                .ctrl         = ( wParam & MK_CONTROL ) != 0,
                .shift        = ( wParam & MK_SHIFT ) != 0,
                .alt          = ( GetAsyncKeyState( VK_MENU ) & 0x8000 ) != 0,
                .super        = ( GetAsyncKeyState( VK_APPS ) & 0x8000 ) != 0,
                .x            = x,
                .y            = y,
                .screenX      = p.x,
                .screenY      = p.y
            };
            window->onMouseButtonReleased( e );
        }
        break;
        case WM_MOUSEWHEEL:
        case WM_MOUSEHWHEEL:
        {
            // The distance the mouse wheel is rotated.
            // A positive value indicates the wheel was rotated forwards (away
            // from the user). A negative value indicates the wheel was rotated
            // backwards (toward the user).
            float zDelta    = GET_WHEEL_DELTA_WPARAM( wParam ) / static_cast<float>( WHEEL_DELTA );
            short keyStates = GET_KEYSTATE_WPARAM( wParam );

            int x = GET_X_LPARAM( lParam );
            int y = GET_Y_LPARAM( lParam );

            // Convert the screen coordinates to client coordinates.
            POINT p { x, y };
            ::ScreenToClient( hWnd, &p );

            MouseWheelEventArgs e {
                .wheelDelta   = zDelta,
                .leftButton   = ( keyStates & MK_LBUTTON ) != 0,
                .middleButton = ( keyStates & MK_MBUTTON ) != 0,
                .rightButton  = ( keyStates & MK_RBUTTON ) != 0,
                .ctrl         = ( keyStates & MK_CONTROL ) != 0,
                .shift        = ( keyStates & MK_SHIFT ) != 0,
                .alt          = ( GetAsyncKeyState( VK_MENU ) & 0x8000 ) != 0,
                .super        = ( GetAsyncKeyState( VK_APPS ) & 0x8000 ) != 0,
                .x            = static_cast<int>( p.x ),
                .y            = static_cast<int>( p.y ),
                .screenX      = x,
                .screenY      = y
            };
            if ( msg == WM_MOUSEWHEEL )
                window->onMouseWheel( e );
            else
                window->onMouseHWheel( e );
        }
        break;
        case WM_MOUSELEAVE:
        {
            window->onMouseLeave();
        }
        break;
        case WM_SIZE:
        {
            ResizeEventArgs e {
                .width  = static_cast<int>( LOWORD( lParam ) ),
                .height = static_cast<int>( HIWORD( lParam ) ),
                .state  = DecodeWindowState( wParam ),
            };

            window->onResize( e );
        }
        break;
        case WM_EXITSIZEMOVE:
            window->onEndResize();
            break;
        default:
            return ::DefWindowProcW( hWnd, msg, wParam, lParam );
        }
    }
    else
    {
        return ::DefWindowProcW( hWnd, msg, wParam, lParam );
    }

    return 0;
}
