#pragma once

#include "Config.hpp"
#include "Image.hpp"

#include <memory>

namespace sr
{
    class ENGINE_API WindowImpl
    {
    public:
        virtual void show() = 0;
        virtual void present(const Image& image) = 0;
        virtual bool popEvent(Event& event) = 0;

    protected:
        friend struct std::default_delete<WindowImpl>; // Allow default delete access to the protected destructor.

        WindowImpl() = default;
        virtual ~WindowImpl() = default;
        WindowImpl(const WindowImpl&) = default;
        WindowImpl(WindowImpl&&) = default;
        WindowImpl& operator=(const WindowImpl&) = default;
        WindowImpl& operator=(WindowImpl&&) = default;
    };
}