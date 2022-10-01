#pragma once

#include "Config.hpp"
#include "WindowImpl.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <string>

namespace Rasterizer
{
    class ENGINE_API WindowWin32 : public WindowImpl
    {
    public:
        WindowWin32(std::string_view title, int width, int height);
        ~WindowWin32();

        void show() override;


    private:
        HWND m_hWnd;
    };
}