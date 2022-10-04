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

void WindowWin32::onKeyPressed( KeyEventArgs& e )
{
    const Event event {
        .type = Event::KeyPressed,
        .key  = e,
    };

    pushEvent( event );
}

void WindowWin32::onKeyReleased( KeyEventArgs& e )
{
    const Event event {
        .type = Event::KeyReleased,
        .key  = e,
    };

    pushEvent( event );
}

void WindowWin32::onResize( ResizeEventArgs& e )
{
    const Event event {
        .type   = Event::Resize,
        .resize = e
    };

    pushEvent( event );
}

void WindowWin32::onEndResize()
{
    RECT clientRect;
    ::GetClientRect( m_hWnd, &clientRect );

    const Event event {
        .type = Event::EndResize,
        .resize {
            .width  = clientRect.right - clientRect.left,
            .height = clientRect.bottom - clientRect.top }
    };

    pushEvent( event );
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

void WindowWin32::processEvents()
{
    MSG msg;
    while ( ::PeekMessageW( &msg, m_hWnd, 0, 0, PM_REMOVE ) )
    {
        ::TranslateMessage( &msg );
        ::DispatchMessageW( &msg );
    }
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
