#pragma once

#include "Config.hpp"
#include "Events.hpp"
#include "WindowImpl.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <queue>
#include <string>

// Forward declaration of Windows callback function.
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

namespace sr
{
class SR_API WindowWin32 : public WindowImpl
{
public:
    WindowWin32( std::wstring_view title, int width, int height );
    ~WindowWin32();

    void show() override;
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
    void onKeyPressed( KeyEventArgs& e );
    void onKeyReleased( KeyEventArgs& e );
    void onResize( ResizeEventArgs& e );
    void onEndResize();

private:
    friend LRESULT CALLBACK ::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

    HWND              m_hWnd;
    std::queue<Event> m_eventQueue;
};
}  // namespace sr