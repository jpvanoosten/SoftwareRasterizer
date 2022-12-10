#include "Window.hpp"

using namespace sr;

#if defined(_WIN32)
#include "WindowWin32.hpp"
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

bool Window::popEvent(Event& event)
{
    return pImpl && pImpl->popEvent(event);
}

void Window::show()
{
    pImpl->show();
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
