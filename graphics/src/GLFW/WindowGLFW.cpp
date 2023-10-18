#include "WindowGLFW.hpp"

#include <fmt/core.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cuchar>  // For mbrtoc32
#include <iostream>

using namespace Graphics;

const char* g_VertexShader = {
#include "../VertexShader.glsl"
};

const char* g_FragmentShader = {
#include "../FragmentShader.glsl"
};

extern void Keyboard_ProcessMessage( const KeyEventArgs& e );
extern void Mouse_ProcessMouseButton( const MouseButtonEventArgs& e );
extern void Mouse_ProcessMouseMove( const MouseMovedEventArgs& e );
extern void Mouse_ProcessVScroll( const MouseWheelEventArgs& e );
extern void Mouse_ProcessHScroll( const MouseWheelEventArgs& e );

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
    }

    ~GLFW_context()
    {
        glfwTerminate();
    }
};

struct ImGui_context
{
    ImGui_context()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport / Platform Windows
        // io.ConfigViewportsNoAutoMerge = true;
        // io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
        {
            style.WindowRounding              = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
        // io.Fonts->AddFontDefault();
        // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
        // IM_ASSERT(font != nullptr);
    }

    ~ImGui_context()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
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

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    window = glfwCreateWindow( width, height, title.data(), nullptr, nullptr );
    makeCurrent();

    int version = gladLoadGL( glfwGetProcAddress );
    if ( version == 0 )
    {
        glfwDestroyWindow( window );
        throw std::runtime_error( "Failed to initialize GLAD." );
    }

    fmt::print( "OpenGL Loaded: {}.{}\n", GLAD_VERSION_MAJOR( version ), GLAD_VERSION_MINOR( version ) );

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
    glfwSetKeyCallback( window, keyCallback );

    // Setup ImGui.
    static ImGui_context _imgui;  // Ensure imgui is initialized once.

    ImGui_ImplGlfw_InitForOpenGL( window, true );
    ImGui_ImplOpenGL3_Init();

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

    // Begin a new ImGui frame.
    beginFrame();

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
    return window;
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

void WindowGLFW::setClearColor( const Color& color ) noexcept
{
    clearColor = color;
}

Color WindowGLFW::getClearColor() const noexcept
{
    return clearColor;
}

void WindowGLFW::clear( const Color& color )
{
    makeCurrent();

    // Bind the default framebuffer.
    glBindFramebuffer( GL_FRONT_AND_BACK, 0 );
    glClearColor( static_cast<float>( color.r ) / 255.0f, static_cast<float>( color.g ) / 255.0f, static_cast<float>( color.b ) / 255.0f, static_cast<float>( color.a ) / 255.0f );
    glClear( GL_COLOR_BUFFER_BIT );

    autoClear = false;
}

void WindowGLFW::present( const Image& image )
{
    makeCurrent();

    if ( autoClear )
        clear( clearColor );

    autoClear = true;

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

    // Rendering.
    ImGui::Render();
    int fbWidth, fbHeight;
    glfwGetFramebufferSize( window, &fbWidth, &fbHeight );
    glViewport( 0, 0, fbWidth, fbHeight );
    ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );

    const ImGuiIO& io = ImGui::GetIO();

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if ( io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable )
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent( backup_current_context );
    }

    glfwSwapBuffers( window );

    // Begin a new frame.
    beginFrame();
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

    return height;
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

void WindowGLFW::beginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
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
    case GLFW_MOUSE_BUTTON_4:
        mouseButton = MouseButton::XButton1;
        break;
    case GLFW_MOUSE_BUTTON_5:
        mouseButton = MouseButton::XButton2;
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
        Mouse_ProcessMouseButton( e );
        w->onMouseButtonPressed( e );
        break;
    case GLFW_RELEASE:
        Mouse_ProcessMouseButton( e );
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

    Mouse_ProcessMouseMove( e );
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

    if ( xOffset != 0.0 )
    {
        e.wheelDelta = static_cast<float>( xOffset );
        Mouse_ProcessHScroll( e );
        w->onMouseHWheel( e );
    }
    if ( yOffset != 0.0 )
    {
        e.wheelDelta = static_cast<float>( yOffset );
        Mouse_ProcessVScroll( e );
        w->onMouseWheel( e );
    }
}

static KeyCode DecodeKeyCode( int key )
{
    KeyCode code = KeyCode::None;

    switch ( key )
    {
    case GLFW_KEY_SPACE:
        code = KeyCode::Space;
        break;
    case GLFW_KEY_APOSTROPHE:
        code = KeyCode::OemQuotes;
        break;
    case GLFW_KEY_COMMA:
        code = KeyCode::OemComma;
        break;
    case GLFW_KEY_MINUS:
        code = KeyCode::OemMinus;
        break;
    case GLFW_KEY_PERIOD:
        code = KeyCode::OemPeriod;
        break;
    case GLFW_KEY_SLASH:
        code = KeyCode::OemQuestion;
        break;
    case GLFW_KEY_0:
        code = KeyCode::D0;
        break;
    case GLFW_KEY_1:
        code = KeyCode::D1;
        break;
    case GLFW_KEY_2:
        code = KeyCode::D2;
        break;
    case GLFW_KEY_3:
        code = KeyCode::D3;
        break;
    case GLFW_KEY_4:
        code = KeyCode::D4;
        break;
    case GLFW_KEY_5:
        code = KeyCode::D5;
        break;
    case GLFW_KEY_6:
        code = KeyCode::D6;
        break;
    case GLFW_KEY_7:
        code = KeyCode::D7;
        break;
    case GLFW_KEY_8:
        code = KeyCode::D8;
        break;
    case GLFW_KEY_9:
        code = KeyCode::D9;
        break;
    case GLFW_KEY_SEMICOLON:
        code = KeyCode::OemSemicolon;
        break;
    case GLFW_KEY_EQUAL:
        code = KeyCode::OemPlus;
        break;
    case GLFW_KEY_A:
        code = KeyCode::A;
        break;
    case GLFW_KEY_B:
        code = KeyCode::B;
        break;
    case GLFW_KEY_C:
        code = KeyCode::C;
        break;
    case GLFW_KEY_D:
        code = KeyCode::D;
        break;
    case GLFW_KEY_E:
        code = KeyCode::E;
        break;
    case GLFW_KEY_F:
        code = KeyCode::F;
        break;
    case GLFW_KEY_G:
        code = KeyCode::G;
        break;
    case GLFW_KEY_H:
        code = KeyCode::H;
        break;
    case GLFW_KEY_I:
        code = KeyCode::I;
        break;
    case GLFW_KEY_J:
        code = KeyCode::J;
        break;
    case GLFW_KEY_K:
        code = KeyCode::K;
        break;
    case GLFW_KEY_L:
        code = KeyCode::L;
        break;
    case GLFW_KEY_M:
        code = KeyCode::M;
        break;
    case GLFW_KEY_N:
        code = KeyCode::N;
        break;
    case GLFW_KEY_O:
        code = KeyCode::O;
        break;
    case GLFW_KEY_P:
        code = KeyCode::P;
        break;
    case GLFW_KEY_Q:
        code = KeyCode::Q;
        break;
    case GLFW_KEY_R:
        code = KeyCode::R;
        break;
    case GLFW_KEY_S:
        code = KeyCode::S;
        break;
    case GLFW_KEY_T:
        code = KeyCode::T;
        break;
    case GLFW_KEY_U:
        code = KeyCode::U;
        break;
    case GLFW_KEY_V:
        code = KeyCode::V;
        break;
    case GLFW_KEY_W:
        code = KeyCode::W;
        break;
    case GLFW_KEY_X:
        code = KeyCode::X;
        break;
    case GLFW_KEY_Y:
        code = KeyCode::Y;
        break;
    case GLFW_KEY_Z:
        code = KeyCode::Z;
        break;
    case GLFW_KEY_LEFT_BRACKET:
        code = KeyCode::OemOpenBrackets;
        break;
    case GLFW_KEY_BACKSLASH:
        code = KeyCode::OemPipe;
        break;
    case GLFW_KEY_RIGHT_BRACKET:
        code = KeyCode::OemCloseBrackets;
        break;
    case GLFW_KEY_GRAVE_ACCENT:
        code = KeyCode::OemTilde;
        break;
    case GLFW_KEY_WORLD_1:
    case GLFW_KEY_WORLD_2:
        // Not used
        break;
    case GLFW_KEY_ESCAPE:
        code = KeyCode::Escape;
        break;
    case GLFW_KEY_ENTER:
        code = KeyCode::Enter;
        break;
    case GLFW_KEY_TAB:
        code = KeyCode::Tab;
        break;
    case GLFW_KEY_BACKSPACE:
        code = KeyCode::Back;
        break;
    case GLFW_KEY_INSERT:
        code = KeyCode::Insert;
        break;
    case GLFW_KEY_DELETE:
        code = KeyCode::Delete;
        break;
    case GLFW_KEY_RIGHT:
        code = KeyCode::Right;
        break;
    case GLFW_KEY_LEFT:
        code = KeyCode::Left;
        break;
    case GLFW_KEY_DOWN:
        code = KeyCode::Down;
        break;
    case GLFW_KEY_UP:
        code = KeyCode::Up;
        break;
    case GLFW_KEY_PAGE_UP:
        code = KeyCode::PageUp;
        break;
    case GLFW_KEY_PAGE_DOWN:
        code = KeyCode::PageDown;
        break;
    case GLFW_KEY_HOME:
        code = KeyCode::Home;
        break;
    case GLFW_KEY_END:
        code = KeyCode::End;
        break;
    case GLFW_KEY_CAPS_LOCK:
        code = KeyCode::CapsLock;
        break;
    case GLFW_KEY_SCROLL_LOCK:
        code = KeyCode::Scroll;
        break;
    case GLFW_KEY_NUM_LOCK:
        code = KeyCode::NumLock;
        break;
    case GLFW_KEY_PRINT_SCREEN:
        code = KeyCode::PrintScreen;
        break;
    case GLFW_KEY_PAUSE:
        code = KeyCode::Pause;
        break;
    case GLFW_KEY_F1:
        code = KeyCode::F1;
        break;
    case GLFW_KEY_F2:
        code = KeyCode::F2;
        break;
    case GLFW_KEY_F3:
        code = KeyCode::F3;
        break;
    case GLFW_KEY_F4:
        code = KeyCode::F4;
        break;
    case GLFW_KEY_F5:
        code = KeyCode::F5;
        break;
    case GLFW_KEY_F6:
        code = KeyCode::F6;
        break;
    case GLFW_KEY_F7:
        code = KeyCode::F7;
        break;
    case GLFW_KEY_F8:
        code = KeyCode::F8;
        break;
    case GLFW_KEY_F9:
        code = KeyCode::F9;
        break;
    case GLFW_KEY_F10:
        code = KeyCode::F10;
        break;
    case GLFW_KEY_F11:
        code = KeyCode::F11;
        break;
    case GLFW_KEY_F12:
        code = KeyCode::F12;
        break;
    case GLFW_KEY_F13:
        code = KeyCode::F13;
        break;
    case GLFW_KEY_F14:
        code = KeyCode::F14;
        break;
    case GLFW_KEY_F15:
        code = KeyCode::F15;
        break;
    case GLFW_KEY_F16:
        code = KeyCode::F16;
        break;
    case GLFW_KEY_F17:
        code = KeyCode::F17;
        break;
    case GLFW_KEY_F18:
        code = KeyCode::F18;
        break;
    case GLFW_KEY_F19:
        code = KeyCode::F19;
        break;
    case GLFW_KEY_F20:
        code = KeyCode::F20;
        break;
    case GLFW_KEY_F21:
        code = KeyCode::F21;
        break;
    case GLFW_KEY_F22:
        code = KeyCode::F22;
        break;
    case GLFW_KEY_F23:
        code = KeyCode::F23;
        break;
    case GLFW_KEY_F24:
        code = KeyCode::F24;
        break;
    case GLFW_KEY_F25:
        code = static_cast<KeyCode>( 0x88 );  // No matching keycode for this key.
        break;
    case GLFW_KEY_KP_0:
        code = KeyCode::NumPad0;
        break;
    case GLFW_KEY_KP_1:
        code = KeyCode::NumPad1;
        break;
    case GLFW_KEY_KP_2:
        code = KeyCode::NumPad2;
        break;
    case GLFW_KEY_KP_3:
        code = KeyCode::NumPad3;
        break;
    case GLFW_KEY_KP_4:
        code = KeyCode::NumPad4;
        break;
    case GLFW_KEY_KP_5:
        code = KeyCode::NumPad5;
        break;
    case GLFW_KEY_KP_6:
        code = KeyCode::NumPad6;
        break;
    case GLFW_KEY_KP_7:
        code = KeyCode::NumPad7;
        break;
    case GLFW_KEY_KP_8:
        code = KeyCode::NumPad8;
        break;
    case GLFW_KEY_KP_9:
        code = KeyCode::NumPad9;
        break;
    case GLFW_KEY_KP_DECIMAL:
        code = KeyCode::Decimal;
        break;
    case GLFW_KEY_KP_DIVIDE:
        code = KeyCode::Divide;
        break;
    case GLFW_KEY_KP_MULTIPLY:
        code = KeyCode::Multiply;
        break;
    case GLFW_KEY_KP_SUBTRACT:
        code = KeyCode::Subtract;
        break;
    case GLFW_KEY_KP_ADD:
        code = KeyCode::Add;
        break;
    case GLFW_KEY_KP_ENTER:
    case GLFW_KEY_KP_EQUAL:
        // No matching KeyCode.
        break;
    case GLFW_KEY_LEFT_SHIFT:
        code = KeyCode::LeftShift;
        break;
    case GLFW_KEY_LEFT_CONTROL:
        code = KeyCode::LeftControl;
        break;
    case GLFW_KEY_LEFT_ALT:
        code = KeyCode::LeftAlt;
        break;
    case GLFW_KEY_LEFT_SUPER:
        code = KeyCode::LeftWindows;
        break;
    case GLFW_KEY_RIGHT_SHIFT:
        code = KeyCode::RightShift;
        break;
    case GLFW_KEY_RIGHT_CONTROL:
        code = KeyCode::RightControl;
        break;
    case GLFW_KEY_RIGHT_ALT:
        code = KeyCode::RightAlt;
        break;
    case GLFW_KEY_RIGHT_SUPER:
        code = KeyCode::RightWindows;
        break;
    case GLFW_KEY_MENU:
        code = KeyCode::AltKey;
        break;
    default:
        code = KeyCode::NoName;  // No matching name for this keycode.
        break;
    }

    return code;
}

// Attempt to decode the glfw key and and scancode into a printable character.
unsigned DecodeCharacter( int key, int scancode )
{
    unsigned c       = 0;
    auto     keyName = glfwGetKeyName( key, scancode );

    if ( keyName )
    {
        std::mbstate_t state {};
        char32_t       c32;
        if ( mbrtoc32( &c32, keyName, 5, &state ) > 0 )
        {
            c = static_cast<unsigned>( c32 );
        }
    }

    return c;
}

void WindowGLFW::keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
    const auto w = static_cast<WindowGLFW*>( glfwGetWindowUserPointer( window ) );

    KeyEventArgs e {
        .code      = DecodeKeyCode( key ),
        .character = DecodeCharacter( key, scancode ),
        .state     = action == GLFW_PRESS ? KeyState::Pressed : KeyState::Released,  // Note: Can also be GLFW_REPEAT
        .ctrl      = ( mods & GLFW_MOD_CONTROL ) != 0,
        .shift     = ( mods & GLFW_MOD_SHIFT ) != 0,
        .alt       = ( mods & GLFW_MOD_ALT ) != 0,
        .super     = ( mods & GLFW_MOD_SUPER ) != 0
    };

    switch ( action )
    {
    case GLFW_PRESS:
        Keyboard_ProcessMessage( e );
        w->onKeyPressed( e );
        break;
    case GLFW_RELEASE:
        Keyboard_ProcessMessage( e );
        w->onKeyReleased( e );
        break;
        // Can also be GLFW_REPEAT, but we don't send key repeat messages.
    }
}
