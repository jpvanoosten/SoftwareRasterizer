#pragma once

#include "Color.hpp"
#include "Config.hpp"

#include <cassert>
#include <filesystem>
#include <memory>

namespace sr
{
struct SR_API Image
{
    static Image fromFile( const std::filesystem::path& fileName );
    static Image fromMemory( const Color* data, uint32_t width, uint32_t height );

    Image();
    Image( uint32_t width, uint32_t height );
    Image( const Image& copy );
    Image( Image&& move ) noexcept;
    ~Image() = default;

    Image& operator=( const Image& image );
    Image& operator=( Image&& image ) noexcept;

    void resize( uint32_t width, uint32_t height );

    void clear( const Color& color );

    const Color& operator()( uint32_t x, uint32_t y ) const
    {
        assert( x < m_width );
        assert( y < m_height );

        return m_data[static_cast<uint64_t>( y ) * m_width + x];
    }

    Color& operator()( uint32_t x, uint32_t y )
    {
        assert( x < m_width );
        assert( y < m_height );

        return m_data[static_cast<uint64_t>( y ) * m_width + x];
    }

    uint32_t getWidth() const noexcept
    {
        return m_width;
    }

    uint32_t getHeight() const noexcept
    {
        return m_height;
    }

    Color* data() noexcept
    {
        return m_data.get();
    }

    const Color* data() const noexcept
    {
        return m_data.get();
    }

private:
    uint32_t                 m_width  = 0u;
    uint32_t                 m_height = 0u;
    std::unique_ptr<Color[]> m_data;
};
}  // namespace sr