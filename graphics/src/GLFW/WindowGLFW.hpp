#pragma once

#include <Graphics/Config.hpp>
#include <Graphics/Events.hpp>
#include <Graphics/WindowImpl.hpp>

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <queue>

namespace Graphics
{
class SR_API WindowGLFW : public WindowImpl
{
public:
    WindowGLFW( std::string_view title, int width, int height );
    ~WindowGLFW() override;

    void show() override;

    WindowHandle getWindowHandle() const noexcept override;

    void setVSync( bool enabled ) override;

    void toggleVSync() override;

    bool isVSync() const noexcept override;

    void setClearColor( const Color& color ) noexcept override;

    Color getClearColor() const noexcept override;

    void clear( const Color& color ) override;

    void present( const Image& image ) override;

    bool popEvent( Event& event ) override;

    int getWidth() const noexcept override;

    int getHeight() const noexcept override;

    glm::ivec2 getSize() const noexcept override;

    void setFullscreen( bool fullscreen ) override;

    bool isFullscreen() const noexcept override;

    void toggleFullscreen() override;

protected:
    void pushEvent( const Event& e );

    void onClose();
    void onKeyPressed( KeyEventArgs& args );
    void onKeyReleased( KeyEventArgs& args );
    void onResize( ResizeEventArgs& args );
    void onEndResize();
    void onMouseEnter( MouseMovedEventArgs& args );
    void onMouseMoved( MouseMovedEventArgs& args );
    void onMouseButtonPressed( MouseButtonEventArgs& args );
    void onMouseButtonReleased( MouseButtonEventArgs& args );
    void onMouseWheel( MouseWheelEventArgs& args );
    void onMouseHWheel( MouseWheelEventArgs& args );
    void onMouseLeave();

private:
    void makeCurrent();
    void beginFrame();

    // GLFW Window callback functions.
    static void closeCallback( GLFWwindow* window );
    static void sizeCallback( GLFWwindow* window, int width, int height );
    static void maximizeCallback( GLFWwindow* window, int maximized );
    static void iconifyCallback( GLFWwindow* window, int iconified );
    static void cursorEnterCallback( GLFWwindow* window, int entered );
    static void mouseButtonCallback( GLFWwindow* window, int button, int action, int mods );
    static void mousePosCallback( GLFWwindow* window, double x, double y );
    static void scrollCallback( GLFWwindow* window, double xOffset, double yOffset );
    static void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods );

    GLFWwindow*       window = nullptr;
    GLuint            m_Texture;        ///< OpenGL texture for pixel transfers.
    GLuint            m_VBO;            ///< Vertex buffer object for the vertices of the quad.
    GLuint            m_IndexBuffer;    ///< Index buffer for draw a quad.
    GLuint            m_VAO;            ///< Vertex Array Object for drawing a fullscreen quad.
    GLuint            m_ShaderProgram;  ///< Shader program.
    std::queue<Event> m_eventQueue;

    int   previousMouseX = 0;
    int   previousMouseY = 0;
    bool  inClientRect   = false;
    bool  vSync          = true;
    bool  fullscreen     = false;
    // Auto clear is disabled if the window is manually cleared by the application.
    bool  autoClear      = true;
    Color clearColor     = Color::Black;
};

}  // namespace Graphics
