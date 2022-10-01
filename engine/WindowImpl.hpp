#pragma once

#include "Config.hpp"

#include <memory>

namespace Rasterizer
{
    class ENGINE_API WindowImpl
    {
    public:
        virtual void show() = 0;

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