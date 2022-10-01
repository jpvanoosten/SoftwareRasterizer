#include "Window.hpp"

using namespace Rasterizer;

#if defined(_WIN32)
#include "WindowWin32.hpp"
using WindowType = WindowWin32;
#endif

Window::Window(std::string_view title, int width, int height)
{
    pImpl = std::make_unique<WindowType>(title, width, height);
}

Window::~Window() = default;

void Window::show()
{
    pImpl->show();
}
