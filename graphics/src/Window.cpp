#include <Graphics/Window.hpp>

using namespace Graphics;

#if defined(_WIN32)
#include "Win32/WindowWin32.hpp"
using WindowType = WindowWin32;
#endif

Window::Window(std::wstring_view title, int width, int height)
{
    create(title, width, height);
}

Window::~Window() = default;
Window::Window(Window&&) noexcept = default;
Window& Window::operator=(Window&&) noexcept = default;

void Window::create(std::wstring_view title, int width, int height)
{
    pImpl = std::make_unique<WindowType>(title, width, height);
}

WindowHandle Window::getWindowHandle() const noexcept
{
    return pImpl->getWindowHandle();
}

bool Window::popEvent(Event& event)
{
    return pImpl && pImpl->popEvent(event);
}

void Window::show()
{
    pImpl->show();
}

void Window::setFullscreen( bool fullscreen )
{
    pImpl->setFullscreen( fullscreen );
}

bool Window::isFullscreen() const noexcept
{
    return pImpl->isFullscreen();
}

void Window::toggleFullscreen()
{
    pImpl->toggleFullscreen();
}

void Window::setVSync( bool enabled )
{
    pImpl->setVSync( enabled );
}

void Window::toggleVSync()
{
    pImpl->toggleVSync();
}

bool Window::isVSync() const noexcept
{
    return pImpl->isVSync();
}

void Window::clear( const Color& color )
{
    pImpl->clear( color );
}

void Window::present(const Image& image)
{
    pImpl->present(image);
}

void Window::destroy()
{
    pImpl.reset();
}

int Window::getWidth() const noexcept
{
    return pImpl->getWidth();
}

int Window::getHeight() const noexcept
{
    return pImpl->getHeight();
}

glm::ivec2 Window::getSize() const noexcept
{
    return pImpl->getSize();
}

Window::operator bool() const
{
    return pImpl != nullptr;
}
