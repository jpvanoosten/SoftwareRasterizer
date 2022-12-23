#pragma once

#include "Config.hpp"
#include "Events.hpp"
#include "WindowImpl.hpp"

#include <GL/glew.h>
#include <GL/wglew.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <windowsx.h>

#include <queue>

// Forward declaration of Windows callback function.
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

namespace sr
{
class SR_API WindowWin32 : public WindowImpl
{
public:
    WindowWin32( std::wstring_view title, int width, int height );
    ~WindowWin32() override;

    void show() override;

    void setVSync( bool enabled ) override;

    bool isVSync() const noexcept override;

    void clear( const Color& color ) override;

    void present( const Image& image ) override;

    bool popEvent( Event& event ) override;

    int getWidth() const noexcept override;

    int getHeight() const noexcept override;

    glm::ivec2 getSize() const noexcept override;

protected:
    void init();

    void processEvents();
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
    friend LRESULT CALLBACK::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

    // Make sure this is the current context on the current thread.
    void makeCurrent();

    void trackMouseEvents() const;

    int  previousMouseX = 0;
    int  previousMouseY = 0;
    bool inClientRect   = false;
    bool vSync          = true;

    HWND              m_hWnd;           ///< Window handle.
    HDC               m_hDC;            ///< Window draw context.
    HGLRC             m_hGLRC;          ///< OpenGL render context.
    GLuint            m_Texture;        ///< OpenGL texture for pixel transfers.
    GLuint            m_VBO;            ///< Vertex buffer object for the vertices of the quad.
    GLuint            m_IndexBuffer;    ///< Index buffer for draw a quad.
    GLuint            m_VAO;            ///< Vertex Array Object for drawing a fullscreen quad.
    GLuint            m_ShaderProgram;  ///< Shader program.
    std::queue<Event> m_eventQueue;
};
}  // namespace sr