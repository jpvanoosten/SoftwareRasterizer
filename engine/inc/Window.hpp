#pragma once

#include "Config.hpp"
#include "Events.hpp"
#include "Image.hpp"

#include <memory>
#include <string>

namespace sr
{
    class WindowImpl;

    class SR_API Window
    {
    public:
        Window();
        Window(std::wstring_view title, int width, int height);
        ~Window();

        // Copies not allowed.
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        // Moves are allowed.
        Window(Window&&) noexcept;
        Window& operator=(Window&&) noexcept;

        // (Re)create the window.
        void create(std::wstring_view title, int width, int height);

        bool popEvent(Event& event);

        void show();
        void present(const Image& image);
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