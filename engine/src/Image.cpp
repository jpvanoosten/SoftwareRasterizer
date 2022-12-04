#include "Image.hpp"
#include "stb_image.h"

#include <Math/AABB.hpp>
#include <Math/Math.hpp>

#include <algorithm>

using namespace sr;
using namespace Math;

Image Image::fromFile( const std::filesystem::path& fileName )
{
    int            x, y, n;
    unsigned char* data = stbi_load( fileName.string().c_str(), &x, &y, &n, STBI_rgb_alpha );
    if ( !data )
        return {};

    // Convert ARGB
    unsigned char* p = data;
    for ( size_t i = 0; i < x * y; ++i )
    {
        unsigned char c = p[0];
        p[0]            = p[2];
        p[2]            = c;
        p += 4;
    }

    Image image = fromMemory( reinterpret_cast<Color*>( data ), x, y );

    stbi_image_free( data );

    return image;
}

Image Image::fromMemory( const Color* data, uint32_t width, uint32_t height )
{
    if ( data == nullptr )
        return {};

    Image image { width, height };
    memcpy_s( image.data(), static_cast<rsize_t>( image.m_width ) * image.m_height * sizeof( Color ), data, static_cast<rsize_t>( width ) * height * sizeof( Color ) );

    return image;
}

Image::Image() = default;

Image::Image( const Image& copy )
{
    resize( copy.m_width, copy.m_height );
    memcpy_s( data(), static_cast<rsize_t>( m_width ) * m_height * sizeof( Color ), copy.data(), static_cast<rsize_t>( copy.m_width ) * copy.m_height * sizeof( Color ) );
}

Image::Image( Image&& move ) noexcept
: m_width { move.m_width }
, m_height { move.m_height }
, m_AABB { { 0, 0, 0 }, { m_width - 1, m_height - 1, 0 } }
, m_data { std::move( move.m_data ) }
{
    move.m_width  = 0u;
    move.m_height = 0u;
}

Image::Image( uint32_t width, uint32_t height )
{
    resize( width, height );
}

Image& Image::operator=( const Image& image )
{
    resize( image.m_width, image.m_height );
    memcpy_s( data(), static_cast<rsize_t>( m_width ) * m_height * sizeof( Color ), image.data(), static_cast<rsize_t>( image.m_width ) * image.m_height * sizeof( Color ) );

    return *this;
}

Image& Image::operator=( Image&& image ) noexcept
{
    m_width  = image.m_width;
    m_height = image.m_height;
    m_AABB   = AABB { { 0, 0, 0 }, { m_width - 1, m_height - 1, 0 } };

    m_data = std::move( image.m_data );

    image.m_width  = 0u;
    image.m_height = 0u;

    return *this;
}

void Image::resize( uint32_t width, uint32_t height )
{
    if ( m_width == width && m_height == height )
        return;

    m_width  = width;
    m_height = height;
    m_AABB   = {
          { 0, 0, 0 },
          { m_width - 1, m_height - 1, 0 }
    };

    m_data = std::make_unique<Color[]>( static_cast<uint64_t>( width ) * height );
}

void Image::clear( const Color& color ) noexcept
{
    Color* p = m_data.get();
    for ( uint32_t i = 0; i < m_width * m_height; ++i )
        p[i] = color;
}

void Image::line( int x0, int y0, int x1, int y1, const Color& color ) noexcept
{
    const int dx = std::abs( x1 - x0 );
    const int dy = -std::abs( y1 - y0 );
    const int sx = x0 < x1 ? 1 : -1;
    const int sy = y0 < y1 ? 1 : -1;

    int err = dx + dy;

    while ( true )
    {
        plot( x0, y0, color );
        const int e2 = err * 2;

        if ( e2 >= dy )
        {
            if ( x0 == x1 )
                break;

            err += dy;
            x0 += sx;
        }
        if ( e2 <= dx )
        {
            if ( y0 == y1 )
                break;

            err += dx;
            y0 += sy;
        }
    }
}

void Image::triangle( const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const Color& color ) noexcept
{
    // Create an AABB for the triangle.
    AABB aabb = AABB::fromTriangle( { p0, 0 }, { p1, 0 }, { p2, 0 } );
    // Clamp the triangle AABB to the screen bounds.
    aabb.clamp( m_AABB );

    glm::vec3 p = aabb.min;
    while ( p.y <= aabb.max.y )
    {
        p.x = aabb.min.x;
        while ( p.x <= aabb.max.x )
        {
            if ( pointInsideTriangle( p, p0, p1, p2 ) )
                plot( static_cast<uint32_t>( p.x ), static_cast<uint32_t>( p.y ), color );

            p.x += 1.0f;
        }
        p.y += 1.0f;
    }
}

const Color& Image::sample( int u, int v, AddressMode addressMode ) const noexcept
{
    const int w = static_cast<int>( m_width );
    const int h = static_cast<int>( m_height );

    switch ( addressMode )
    {
    case AddressMode::Wrap:
    {
        u = u % w;
        v = v % h;
    }
    break;
    case AddressMode::Mirror:
    {
        const int U = u / w;
        const int V = v / h;

        u = ( U % 2 == 0 ) ? u % w : w - ( u % w );
        v = ( V % 2 == 0 ) ? v % h : h - ( v % h );
    }
    break;
    case AddressMode::Clamp:
    {
        u = std::clamp( u, 0, w );
        v = std::clamp( v, 0, h );
    }
    break;
    }

    assert( u > 0 && u < w );
    assert( v > 0 && v < h );

    return m_data[static_cast<uint64_t>( v ) * m_width + u];
}
