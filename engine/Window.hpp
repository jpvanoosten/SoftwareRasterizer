#pragma once

#include "Config.hpp"

#include <memory>
#include <string>

namespace Rasterizer
{
    class WindowImpl;

    class ENGINE_API Window
    {
    public:
        Window(std::string_view title, int width, int height);
        ~Window();

        void show();
    protected:

    private:
        std::unique_ptr<WindowImpl> pImpl;
    };
}