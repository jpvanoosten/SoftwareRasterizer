#include "WindowWin32.hpp"

#include <iostream>

using namespace Rasterizer;

WindowWin32::WindowWin32(std::string_view title, int width, int height)
{
    std::cout << "Hello from Windows!" << std::endl;
}

WindowWin32::~WindowWin32()
{
    // TODO: Destroy window.
}

void WindowWin32::show()
{
    // TODO: Show the window.
}