#include <Graphics/Mouse.hpp>
#include <Graphics/Window.hpp>

#include <GLFW/glfw3.h>

#include <mutex>

using namespace Graphics;

static GLFWwindow* g_hWnd = nullptr;
static MouseState  g_globalState {};
static std::mutex  g_stateMutex;

// Local mouse state is updated in ProcessEvents.
thread_local MouseState localState {};
thread_local int        relativeX { 0 };
thread_local int        relativeY { 0 };
thread_local bool       inFocus { true };

bool Mouse::isConnected()
{
    return true;
}

bool Mouse::isVisible( const Window& window )
{
    return glfwGetInputMode( window.getWindowHandle(), GLFW_CURSOR ) == GLFW_CURSOR_NORMAL;
}

void Mouse::setVisible( const Window& window, bool visible )
{
    glfwSetInputMode( window.getWindowHandle(), GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN );
}

void Mouse::lockToWindow( const Window& window )
{
    GLFWwindow* hWnd = window.getWindowHandle();
    if ( hWnd != g_hWnd )
    {
        g_hWnd = hWnd;
        glfwSetInputMode( g_hWnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    }
}

void Mouse::unlock()
{
    if ( g_hWnd != nullptr )
    {
        glfwSetInputMode( g_hWnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
        g_hWnd = nullptr;
    }
}

bool Mouse::isLocked()
{
    return g_hWnd != nullptr;
}

MouseState Mouse::getState()
{
    return g_globalState;
}

glm::ivec2 Mouse::getPosition()
{
    throw std::runtime_error( "This function is not supported with GLFW backed." );
}

glm::ivec2 Mouse::getPosition( const Window& window )
{
    double x, y;
    glfwGetCursorPos( window.getWindowHandle(), &x, &y );

    return {
        static_cast<int>( x ), static_cast<int>( y )
    };
}

void Mouse::setPosition( const glm::ivec2& pos )
{
    throw std::runtime_error( "This function is not supported with GLFW backed." );
}

void Mouse::setPosition( const glm::ivec2& pos, const Window& window )
{
    glfwSetCursorPos( window.getWindowHandle(), pos.x, pos.y );
}

void Mouse_ProcessMouseButton( const MouseButtonEventArgs& e )
{
    switch ( e.button )
    {
    case MouseButton::Left:
        g_globalState.leftButton = e.state == ButtonState::Pressed;
        break;
    case MouseButton::Middle:
        g_globalState.middleButton = e.state == ButtonState::Pressed;
        break;
    case MouseButton::Right:
        g_globalState.rightButton = e.state == ButtonState::Pressed;
        break;
    case MouseButton::XButton1:
        g_globalState.xButton1 = e.state == ButtonState::Pressed;
        break;
    case MouseButton::XButton2:
        g_globalState.xButton2 = e.state == ButtonState::Pressed;
        break;
    }
}

void Mouse_ProcessMouseMove(const MouseMovedEventArgs& e)
{
    g_globalState.x = e.x;
    g_globalState.y = e.y;
    g_globalState.screenY = e.screenX;
    g_globalState.screenY = e.screenY;
}

void Mouse_ProcessVScroll(const MouseWheelEventArgs& e)
{
    g_globalState.vScrollWheel += e.wheelDelta;
}

void Mouse_ProcessHScroll( const MouseWheelEventArgs& e)
{
    g_globalState.hScrollWheel += e.wheelDelta;
}