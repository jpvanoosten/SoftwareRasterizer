#include "WindowGLFW.hpp"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <fmt/core.h>

#include <iostream>

using namespace Graphics;

const char* g_VertexShader = {
#include "VertexShader.glsl"
};

const char* g_FragmentShader = {
#include "FragmentShader.glsl"
};

// This struct ensure GLFW is initialized once and destroyed once.
struct GLFW_context
{
    static void error_callback( int error, const char* description )
    {
        fmt::print( stderr, "GLFW Error [{}]: {}\n", error, description );
    }

    GLFW_context()
    {
        glfwSetErrorCallback( error_callback );

        if ( glfwInit() != GLFW_TRUE )
        {
            throw std::runtime_error( "Failed to initialize GLFW." );
        }

        // Create a temporary window so that we have an OpenGL context for extension loading.
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
#if _DEBUG
        glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
#endif

        auto window = glfwCreateWindow( 1, 1, "_TEMP", nullptr, nullptr );
        glfwMakeContextCurrent( window );

        int version = gladLoadGL( glfwGetProcAddress );
        if ( version == 0 )
        {
            glfwDestroyWindow( window );
            throw std::runtime_error( "Failed to initialize GLAD." );
        }

        fmt::print( "OpenGL Loaded: {}.{}\n", GLAD_VERSION_MAJOR( version ), GLAD_VERSION_MINOR( version ) );

        // Destroy the temporary window.
        glfwDestroyWindow( window );
    }

    ~GLFW_context()
    {
        glfwTerminate();
    }
};

WindowGLFW::WindowGLFW( std::string_view title, int width, int height )
{
    static GLFW_context _glfw;  // Will only be called once to initialize GLFW.

    const auto monitor = glfwGetPrimaryMonitor();
    const auto mode    = glfwGetVideoMode( monitor );

    glfwWindowHint( GLFW_RED_BITS, mode->redBits );
    glfwWindowHint( GLFW_GREEN_BITS, mode->greenBits );
    glfwWindowHint( GLFW_BLUE_BITS, mode->blueBits );
    glfwWindowHint( GLFW_REFRESH_RATE, mode->refreshRate );

    window = glfwCreateWindow( width, height, title.data(), nullptr, nullptr );

    // Set the user pointer on the window to retrieve the window in callback functions.
    glfwSetWindowUserPointer( window, this );

    // Setup callback functions
    glfwSetWindowCloseCallback( window, closeCallback );
    glfwSetWindowSizeCallback( window, sizeCallback );
    glfwSetWindowMaximizeCallback( window, maximizeCallback );
    glfwSetWindowIconifyCallback( window, iconifyCallback );
    glfwSetCursorEnterCallback( window, cursorEnterCallback );
    glfwSetMouseButtonCallback( window, mouseButtonCallback );
    glfwSetCursorPosCallback( window, mousePosCallback );
    glfwSetScrollCallback( window, scrollCallback );

    makeCurrent();

    // Create an OpenGL texture for pixel operations.
    glGenTextures( 1, &m_Texture );
    glTextureParameteri( m_Texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTextureParameteri( m_Texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTextureParameteri( m_Texture, GL_TEXTURE_MAX_LEVEL, 0 );

    // Create a full-screen quad.
    struct Vertex
    {
        glm::vec2 pos;
        glm::vec2 uv;
    };

    const Vertex verts[] = {
        { { -1, 1 }, { 0, 0 } },  // Top-left.
        { { 1, 1 }, { 1, 0 } },   // Top-right.
        { { 1, -1 }, { 1, 1 } },  // Bottom-right.
        { { -1, -1 }, { 0, 1 } }  // Bottom-left.
    };
    const uint8_t indices[] = {
        0, 1, 2,  // First triangle.
        2, 3, 0   // Second triangle.
    };

    glGenVertexArrays( 1, &m_VAO );
    glBindVertexArray( m_VAO );

    glGenBuffers( 1, &m_VBO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( verts ), verts, GL_STATIC_DRAW );

    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, pos ) );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, uv ) );
    glEnableVertexAttribArray( 1 );

    glGenBuffers( 1, &m_IndexBuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    glBindVertexArray( 0 );

    // Load shaders.
    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShader, 1, &g_VertexShader, nullptr );
    glCompileShader( vertexShader );

    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShader, 1, &g_FragmentShader, nullptr );
    glCompileShader( fragmentShader );

    m_ShaderProgram = glCreateProgram();
    glAttachShader( m_ShaderProgram, vertexShader );
    glAttachShader( m_ShaderProgram, fragmentShader );
    glLinkProgram( m_ShaderProgram );

    // Check for success.
    {
        GLint success;
        glGetProgramiv( m_ShaderProgram, GL_LINK_STATUS, &success );
        if ( !success )
        {
            GLchar infoLog[512];
            glGetProgramInfoLog( m_ShaderProgram, 512, nullptr, infoLog );
            std::cerr << "Failed to link program: " << infoLog << std::endl;
        }
    }

    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

    WindowGLFW::setVSync( vSync );
}

WindowGLFW::~WindowGLFW()
{
    glfwDestroyWindow( window );
}

void WindowGLFW::show()
{
    glfwShowWindow( window );
}

WindowHandle WindowGLFW::getWindowHandle() const noexcept
{
    // TODO: Handle different platforms?
    return glfwGetWin32Window( window );
}

void WindowGLFW::setVSync( bool enabled )
{
    makeCurrent();
    glfwSwapInterval( enabled ? 1 : 0 );
    vSync = enabled;
}

void WindowGLFW::toggleVSync()
{
    setVSync( !vSync );
}

bool WindowGLFW::isVSync() const noexcept
{
    return vSync;
}

void WindowGLFW::clear( const Color& color )
{
    makeCurrent();

    // Bind the default framebuffer.
    glBindFramebuffer( GL_FRONT_AND_BACK, 0 );
    glClearColor( static_cast<float>( color.r ) / 255.0f, static_cast<float>( color.g ) / 255.0f, static_cast<float>( color.b ) / 255.0f, static_cast<float>( color.a ) / 255.0f );
    glClear( GL_COLOR_BUFFER_BIT );
}

void WindowGLFW::present( const Image& image )
{
    makeCurrent();

    // Copy the image data to the texture
    glBindTexture( GL_TEXTURE_2D, m_Texture );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>( image.getWidth() ), static_cast<GLsizei>( image.getHeight() ), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data() );
    glTextureParameteri( m_Texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTextureParameteri( m_Texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    // Center the image on screen and maintain the aspect ratio.
    const float aspectRatio = static_cast<float>( image.getWidth() ) / static_cast<float>( image.getHeight() );
    const float scaleWidth  = static_cast<float>( getWidth() ) / static_cast<float>( image.getWidth() );
    const float scaleHeight = static_cast<float>( getHeight() ) / static_cast<float>( image.getHeight() );

    int width, height;

    if ( scaleWidth < scaleHeight )
    {
        width  = getWidth();
        height = static_cast<int>( static_cast<float>( width ) / aspectRatio );
    }
    else
    {
        height = getHeight();
        width  = static_cast<int>( static_cast<float>( height ) * aspectRatio );
    }
    const int x = ( getWidth() - width ) / 2;
    const int y = ( getHeight() - height ) / 2;

    glViewport( x, y, width, height );

    glUseProgram( m_ShaderProgram );
    glBindVertexArray( m_VAO );
    glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr );

    // TODO: ImGui

    glfwSwapBuffers( window );
}

bool WindowGLFW::popEvent( Event& event )
{
    if ( m_eventQueue.empty() )
        glfwPollEvents();

    if ( m_eventQueue.empty() )
        return false;

    event = m_eventQueue.front();
    m_eventQueue.pop();

    return true;
}

int WindowGLFW::getWidth() const noexcept
{
    int width, height;
    glfwGetWindowSize( window, &width, &height );

    return width;
}

int WindowGLFW::getHeight() const noexcept
{
    int width, height;
    glfwGetWindowSize( window, &width, &height );

    return width;
}

glm::ivec2 WindowGLFW::getSize() const noexcept
{
    int width, height;
    glfwGetWindowSize( window, &width, &height );

    return { width, height };
}

void WindowGLFW::setFullscreen( bool _fullscreen )
{
    if ( fullscreen != _fullscreen )
    {
        fullscreen = _fullscreen;

        if ( fullscreen )
        {
            glfwSetWindowAttrib( window, GLFW_DECORATED, GLFW_FALSE );
            glfwSetWindowAttrib( window, GLFW_FLOATING, GLFW_TRUE );
            glfwMaximizeWindow( window );
        }
        else
        {
            glfwRestoreWindow( window );
            glfwSetWindowAttrib( window, GLFW_FLOATING, GLFW_FALSE );
            glfwSetWindowAttrib( window, GLFW_DECORATED, GLFW_TRUE );
        }
    }
}

bool WindowGLFW::isFullscreen() const noexcept
{
    return fullscreen;
}

void WindowGLFW::toggleFullscreen()
{
    setFullscreen( !fullscreen );
}

void WindowGLFW::pushEvent( const Event& e )
{
    m_eventQueue.push( e );
}

void WindowGLFW::onClose()
{
    constexpr Event e { .type = Event::Close };
    pushEvent( e );
}

void WindowGLFW::onKeyPressed( KeyEventArgs& args )
{
    const Event e {
        .type = Event::KeyPressed,
        .key  = args,
    };
    pushEvent( e );
}

void WindowGLFW::onKeyReleased( KeyEventArgs& args )
{
    const Event e {
        .type = Event::KeyReleased,
        .key  = args,
    };
    pushEvent( e );
}

void WindowGLFW::onResize( ResizeEventArgs& args )
{
    const Event e {
        .type   = Event::Resize,
        .resize = args
    };
    pushEvent( e );
}

void WindowGLFW::onEndResize()
{
    auto size = getSize();

    const Event e {
        .type = Event::EndResize,
        .resize {
            .width  = size.x,
            .height = size.y }
    };

    pushEvent( e );
}

void WindowGLFW::onMouseEnter( MouseMovedEventArgs& args )
{
    const Event e {
        .type      = Event::MouseEnter,
        .mouseMove = args
    };

    pushEvent( e );
}

void WindowGLFW::onMouseMoved( MouseMovedEventArgs& args )
{
    if ( !inClientRect )
    {
        inClientRect = true;

        previousMouseX = args.screenX;
        previousMouseY = args.screenY;

        // Also invoke a mouse enter events.
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

void WindowGLFW::onMouseButtonPressed( MouseButtonEventArgs& args )
{
    const Event e {
        .type        = Event::MouseButtonPressed,
        .mouseButton = args
    };
    pushEvent( e );
}

void WindowGLFW::onMouseButtonReleased( MouseButtonEventArgs& args )
{
    const Event e {
        .type        = Event::MouseButtonReleased,
        .mouseButton = args
    };
    pushEvent( e );
}

void WindowGLFW::onMouseWheel( MouseWheelEventArgs& args )
{
    const Event e {
        .type       = Event::MouseWheel,
        .mouseWheel = args
    };
    pushEvent( e );
}

void WindowGLFW::onMouseHWheel( MouseWheelEventArgs& args )
{
    const Event e {
        .type       = Event::MouseHWheel,
        .mouseWheel = args
    };
    pushEvent( e );
}

void WindowGLFW::onMouseLeave()
{
    // Mouse left client area.
    inClientRect = false;

    constexpr Event e {
        .type = Event::MouseLeave
    };
    pushEvent( e );
}

void WindowGLFW::makeCurrent()
{
    glfwMakeContextCurrent( window );
}

void WindowGLFW::closeCallback( GLFWwindow* window )
{
    auto w = static_cast<WindowGLFW*>( glfwGetWindowUserPointer( window ) );
    w->onClose();
}

void WindowGLFW::sizeCallback( GLFWwindow* window, int width, int height )
{
    const auto w = static_cast<WindowGLFW*>( glfwGetWindowUserPointer( window ) );

    ResizeEventArgs e {
        .width  = width,
        .height = height,
        .state  = WindowState::Resized
    };

    w->onResize( e );
}

void WindowGLFW::maximizeCallback( GLFWwindow* window, int maximized )
{
    const auto w    = static_cast<WindowGLFW*>( glfwGetWindowUserPointer( window ) );
    const auto size = w->getSize();

    ResizeEventArgs e {
        .width  = size.x,
        .height = size.y,
        .state  = maximized ? WindowState::Maximized : WindowState::Restored
    };

    w->onResize( e );
}

void WindowGLFW::iconifyCallback( GLFWwindow* window, int iconified )
{
    const auto w    = static_cast<WindowGLFW*>( glfwGetWindowUserPointer( window ) );
    const auto size = w->getSize();

    ResizeEventArgs e {
        .width  = size.x,
        .height = size.y,
        .state  = iconified ? WindowState::Minimized : WindowState::Restored
    };

    w->onResize( e );
}

void WindowGLFW::cursorEnterCallback( GLFWwindow* window, int entered )
{
    const auto w = static_cast<WindowGLFW*>( glfwGetWindowUserPointer( window ) );

    if ( entered )
    {
        // Do nothing until a mouseMove event is fired.
    }
    else
    {
        w->onMouseLeave();
    }
}

static MouseButton DecodeMouseButton( int button )
{
    MouseButton mouseButton = MouseButton::None;

    switch ( button )
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        mouseButton = MouseButton::Left;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        mouseButton = MouseButton::Right;
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        mouseButton = MouseButton::Middle;
        break;
    }

    return mouseButton;
}

void WindowGLFW::mouseButtonCallback( GLFWwindow* window, int button, int action, int mods )
{
    const auto w = static_cast<WindowGLFW*>( glfwGetWindowUserPointer( window ) );

    double clientX, clientY;
    glfwGetCursorPos( window, &clientX, &clientY );

    int windowX, windowY;
    glfwGetWindowPos( window, &windowX, &windowY );

    MouseButtonEventArgs e {
        .button       = DecodeMouseButton( button ),
        .state        = action == GLFW_PRESS ? ButtonState::Pressed : ButtonState::Released,
        .leftButton   = glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS,
        .middleButton = glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_MIDDLE ) == GLFW_PRESS,
        .rightButton  = glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_RIGHT ) == GLFW_PRESS,
        .ctrl         = ( mods & GLFW_MOD_CONTROL ) != 0,
        .shift        = ( mods & GLFW_MOD_SHIFT ) != 0,
        .alt          = ( mods & GLFW_MOD_ALT ) != 0,
        .super        = ( mods & GLFW_MOD_SUPER ) != 0,
        .x            = static_cast<int>( floor( clientX ) ),
        .y            = static_cast<int>( floor( clientY ) ),
        .screenX      = windowX + static_cast<int>( floor( clientX ) ),
        .screenY      = windowY + static_cast<int>( floor( clientY ) )
    };

    switch ( action )
    {
    case GLFW_PRESS:
        w->onMouseButtonPressed( e );
        break;
    case GLFW_RELEASE:
        w->onMouseButtonReleased( e );
        break;
    }
}

void WindowGLFW::mousePosCallback( GLFWwindow* window, double x, double y )
{
    const auto w = static_cast<WindowGLFW*>( glfwGetWindowUserPointer( window ) );

    int windowX, windowY;
    glfwGetWindowPos( window, &windowX, &windowY );

    MouseMovedEventArgs e {
        .leftButton   = glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS,
        .middleButton = glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_MIDDLE ) == GLFW_PRESS,
        .rightButton  = glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_RIGHT ) == GLFW_PRESS,
        .ctrl         = glfwGetKey( window, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_RIGHT_CONTROL ) == GLFW_PRESS,
        .shift        = glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_RIGHT_SHIFT ) == GLFW_PRESS,
        .alt          = glfwGetKey( window, GLFW_KEY_LEFT_ALT ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_RIGHT_ALT ) == GLFW_PRESS,
        .super        = glfwGetKey( window, GLFW_KEY_LEFT_SUPER ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_RIGHT_SUPER ) == GLFW_PRESS,
        .x            = static_cast<int>( floor( x ) ),
        .y            = static_cast<int>( floor( y ) ),
        .screenX      = windowX + static_cast<int>( floor( x ) ),
        .screenY      = windowY + static_cast<int>( floor( y ) )
    };

    w->onMouseMoved( e );
}

void WindowGLFW::scrollCallback( GLFWwindow* window, double xOffset, double yOffset )
{
    const auto w = static_cast<WindowGLFW*>( glfwGetWindowUserPointer( window ) );

    double clientX, clientY;
    glfwGetCursorPos( window, &clientX, &clientY );

    int windowX, windowY;
    glfwGetWindowPos( window, &windowX, &windowY );

    MouseWheelEventArgs e {
        .leftButton   = glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS,
        .middleButton = glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_MIDDLE ) == GLFW_PRESS,
        .rightButton  = glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_RIGHT ) == GLFW_PRESS,
        .ctrl         = glfwGetKey( window, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_RIGHT_CONTROL ) == GLFW_PRESS,
        .shift        = glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_RIGHT_SHIFT ) == GLFW_PRESS,
        .alt          = glfwGetKey( window, GLFW_KEY_LEFT_ALT ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_RIGHT_ALT ) == GLFW_PRESS,
        .super        = glfwGetKey( window, GLFW_KEY_LEFT_SUPER ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_RIGHT_SUPER ) == GLFW_PRESS,
        .x            = static_cast<int>( floor( clientX ) ),
        .y            = static_cast<int>( floor( clientY ) ),
        .screenX      = windowX + static_cast<int>( floor( clientX ) ),
        .screenY      = windowY + static_cast<int>( floor( clientY ) )
    };

    if (xOffset != 0.0)
    {
        e.wheelDelta = static_cast<float>( xOffset );
        w->onMouseHWheel( e );
    }
    if (yOffset != 0.0)
    {
        e.wheelDelta = static_cast<float>( yOffset );
        w->onMouseWheel( e );
    }
}
