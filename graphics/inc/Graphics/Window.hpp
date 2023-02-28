#pragma once

#include "Config.hpp"
#include "Events.hpp"
#include "Image.hpp"
#include "WindowHandle.hpp"

#include <glm/vec2.hpp>

#include <memory>

namespace Graphics
{
class WindowImpl;

class SR_API Window
{
public:
    Window();
    Window( std::wstring_view title, int width, int height );
    ~Window();

    // Copies not allowed.
    Window( const Window& )            = delete;
    Window& operator=( const Window& ) = delete;

    // Moves are allowed.
    Window( Window&& ) noexcept;
    Window& operator=( Window&& ) noexcept;

    /// <summary>
    /// Create the window instance.
    /// </summary>
    /// <param name="title">The title to display in the window's title bar.</param>
    /// <param name="width">The initial width of the window.</param>
    /// <param name="height">The initial height of the window.</param>
    void create( std::wstring_view title, int width, int height );

    /// <summary>
    /// Get an OS window handle.
    /// </summary>
    /// <returns>The OS window handle.</returns>
    WindowHandle getWindowHandle() const noexcept;

    /// <summary>
    /// Pop an event from the window's event queue.
    /// </summary>
    /// <param name="event">A reference to an Event object that will be filled in with the next event in the event queue.</param>
    /// <returns>`true` if an event was popped from the event queue, or `false` if there are no events in the queue.</returns>
    bool popEvent( Event& event );

    /// <summary>
    /// Show the window.
    /// </summary>
    void show();

    /// <summary>
    /// Set the fullscreen state of the Window.
    /// </summary>
    /// <param name="fullscreen">`true` to set the window to fullscreen, `false` to restore to Windowed mode.</param>
    void setFullscreen( bool fullscreen );

    /// <summary>
    /// Get the fullscreen state of the window.
    /// </summary>
    /// <returns>`true` if the window is in a fullscreen state, `false` otherwise.</returns>
    bool isFullscreen() const noexcept;

    /// <summary>
    /// Toggle the fullscreen state.
    /// </summary>
    void toggleFullscreen();

    /// <summary>
    /// Enable or disable v-sync.
    /// </summary>
    /// <param name="enabled">`true` to enable v-sync, or `false` to disable it.</param>
    void setVSync( bool enabled );

    /// <summary>
    /// Toggles vsync on/off.
    /// </summary>
    void toggleVSync();

    /// <summary>
    /// Check to see if v-sync is enabled.
    /// </summary>
    /// <returns>`true` if v-sync is enabled, or `false` otherwise.</returns>
    bool isVSync() const noexcept;

    /// <summary>
    /// Clear the window contents.
    /// </summary>
    /// <param name="color">The color to clear the window to.</param>
    void clear( const Color& color );

    /// <summary>
    /// Present the image to the screen.
    /// </summary>
    /// <param name="image">The image to present.</param>
    void present( const Image& image );

    /// <summary>
    /// Destroy the window.
    /// Note: This deletes the pointer to the window implementation and it
    /// is an error to try to use the window after it has been destroyed.
    /// </summary>
    void destroy();

    /// <summary>
    /// Get the width (in pixels) of the client area of the window.
    /// </summary>
    /// <returns>The width (in pixels) of the client area of the window.</returns>
    int getWidth() const noexcept;

    /// <summary>
    /// Get the height (in pixels) of the client area of the window.
    /// </summary>
    /// <returns>The height (in pixels) of the client area of the window.</returns>
    int getHeight() const noexcept;

    /// <summary>
    /// Get the width and height (in pixels) of the client area of the window.
    /// </summary>
    /// <returns>The width and height (in pixels) of the client area of the window.</returns>
    glm::ivec2 getSize() const noexcept;

    /// <summary>
    /// Check to see if the window is valid.
    /// </summary>
    /// <returns>`true` if the window pointer is valid, `false` if the window was never created or has already been destroyed.</returns>
    explicit operator bool() const;

private:
    std::unique_ptr<WindowImpl> pImpl;
};
}  // namespace Graphics