#pragma once

#include "Color.hpp"
#include "Config.hpp"

#include <Math/AABB.hpp>

#include <cassert>
#include <filesystem>
#include <memory>

#include <glm/vec2.hpp>

namespace sr
{

/// <summary>
/// Address modes used for texture sampling.
/// </summary>
enum class AddressMode
{
    Wrap,    ///< Tile the texture.
    Mirror,  ///< Flip the texture coordinates at integer boundaries.
    Clamp,   ///< Clamp texture coordinates in the range 0..1.
};

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

    /// <summary>
    /// Clear the image to a single color.
    /// </summary>
    /// <param name="color">The color to clear the screen to.</param>
    void clear( const Color& color ) noexcept;

    void line( int x0, int y0, int x1, int y1, const Color& color ) noexcept;

    void line( const glm::ivec2& p0, const glm::ivec2& p1, const Color& color ) noexcept
    {
        line( p0.x, p0.y, p1.x, p1.y, color );
    }

    void line( const glm::vec2& p0, const glm::vec2& p1, const Color& color ) noexcept
    {
        line( static_cast<int>( p0.x ), static_cast<int>( p0.y ), static_cast<int>( p1.x ), static_cast<int>( p0.y ), color );
    }

    /// <summary>
    /// Plot a 2D triangle.
    /// </summary>
    /// <param name="p0">The first triangle coordinate.</param>
    /// <param name="p1">The second triangle coordinate.</param>
    /// <param name="p2">The third triangle coordinate.</param>
    /// <param name="color">The triangle color.</param>
    void triangle( const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const Color& color ) noexcept;

    /// <summary>
    /// Plot a single pixel to the image. Out-of-bounds coordinates are discarded.
    /// </summary>
    /// <param name="x">The x-coordinate to plot.</param>
    /// <param name="y">The y-coordinate to plot.</param>
    /// <param name="color">The color of the pixel to plot.</param>
    void plot( uint32_t x, uint32_t y, const Color& color ) noexcept
    {
        if ( x >= m_width || y >= m_height )
            return;

        m_data[static_cast<uint64_t>( y ) * m_width + x] = color;
    }

    /// <summary>
    /// Sample the image at integer coordinates.
    /// </summary>
    /// <param name="u">The U texture coordinate.</param>
    /// <param name="v">The V texture coordinate.</param>
    /// <param name="addressMode">Determines how to apply out-of-bounds texture coordinates.</param>
    /// <returns>The color of the texel at the given UV coordinates.</returns>
    const Color& sample( int u, int v, AddressMode addressMode = AddressMode::Wrap ) const noexcept;

    /// <summary>
    /// Sample the image at integer coordinates.
    /// </summary>
    /// <param name="uv">The texture coordinates.</param>
    /// <param name="addressMode">The address mode to use during sampling.</param>
    /// <returns>The color of the texel at the given UV coordinates.</returns>
    const Color& sample( const glm::ivec2& uv, AddressMode addressMode = AddressMode::Wrap ) const noexcept
    {
        return sample( uv.x, uv.y, addressMode );
    }

    /// <summary>
    /// Sample the image using normalized texture coordinates (in the range from [0..1]).
    /// </summary>
    /// <param name="u">The normalized U texture coordinate.</param>
    /// <param name="v">The normalized V texture coordinate.</param>
    /// <param name="addressMode">The addressing mode to use during sampling.</param>
    /// <returns>The color of the texel at the given UV texture coordinates.</returns>
    const Color& sample( float u, float v, AddressMode addressMode = AddressMode::Wrap ) const noexcept
    {
        return sample( static_cast<int>( u * static_cast<float>(m_width) ), static_cast<int>( v * static_cast<float>(m_height) ), addressMode );
    }

    /// <summary>
    /// Sample the image using normalized texture coordinates (in the range from [0..1]).
    /// </summary>
    /// <param name="uv">The normalized texture coordinates.</param>
    /// <param name="addressMode">The addressing mode to use during sampling.</param>
    /// <returns>The color of the texel at the given UV texture coordinates.</returns>
    const Color& sample( const glm::vec2& uv, AddressMode addressMode = AddressMode::Wrap ) const noexcept
    {
        return sample( uv.x, uv.y, addressMode );
    }

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
    // Axis-aligned bounding box used for screen clipping.
    Math::AABB               m_AABB;
    std::unique_ptr<Color[]> m_data;
};
}  // namespace sr