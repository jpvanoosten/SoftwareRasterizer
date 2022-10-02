#pragma once

#include "Config.hpp"
#include "Events.hpp"

#include <memory>
#include <string>

namespace Rasterizer
{
    class WindowImpl;

    class ENGINE_API Window
    {
    public:
        Window(std::wstring_view title, int width, int height);
        ~Window();

        bool popEvent(Event& event);

        void show();
        void destroy();

        /// <summary>
        /// Check to see if the window is valid.
        /// </summary>
        explicit operator bool() const;

    protected:

    private:
        std::unique_ptr<WindowImpl> pImpl;
    };
}