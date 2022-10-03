#include "Window.hpp"

using namespace sr;

#if defined(_WIN32)
#include "WindowWin32.hpp"
using WindowType = WindowWin32;
#endif

Window::Window(std::wstring_view title, int width, int height)
{
    pImpl = std::make_unique<WindowType>(title, width, height);
}

Window::~Window() = default;

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

Window::operator bool() const
{
    return pImpl != nullptr;
}
