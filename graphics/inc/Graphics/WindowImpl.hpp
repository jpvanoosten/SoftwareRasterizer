#pragma once

#include "Config.hpp"
#include "Image.hpp"
#include "WindowHandle.hpp"

#include <glm/vec2.hpp>

#include <memory>

namespace Graphics
{
class SR_API WindowImpl
{
public:
    virtual void         show()                           = 0;
    virtual WindowHandle getWindowHandle() const noexcept = 0;
    virtual void         setVSync( bool enabled )         = 0;
    virtual bool         isVSync() const noexcept         = 0;
    virtual void         clear( const Color& color )      = 0;
    virtual void         present( const Image& image )    = 0;
    virtual bool         popEvent( Event& event )         = 0;
    virtual int          getWidth() const noexcept        = 0;
    virtual int          getHeight() const noexcept       = 0;
    virtual glm::ivec2   getSize() const noexcept         = 0;
    virtual void         setFullscreen( bool fullscreen ) = 0;
    virtual bool         isFullscreen() const noexcept    = 0;
    virtual void         toggleFullscreen()               = 0;
    virtual void         toggleVSync()                    = 0;

protected:
    friend struct std::default_delete<WindowImpl>;  // Allow default delete access to the protected destructor.

    WindowImpl()                               = default;
    virtual ~WindowImpl()                      = default;
    WindowImpl( const WindowImpl& )            = default;
    WindowImpl( WindowImpl&& )                 = default;
    WindowImpl& operator=( const WindowImpl& ) = default;
    WindowImpl& operator=( WindowImpl&& )      = default;
};
}  // namespace Graphics