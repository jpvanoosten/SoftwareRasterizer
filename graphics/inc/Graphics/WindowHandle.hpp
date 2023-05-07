#pragma once

#if defined( _WIN32 )
struct HWND__;  // Forward declare the HWND__ struct to avoid including Windows.h.
#endif

namespace Graphics
{
#if defined( _WIN32 )
using WindowHandle = HWND__*;
#else
using WindowHandle = void*;
#endif
}  // namespace Graphics