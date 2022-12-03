#include "Image.hpp"

#include "stb_image.h"

using namespace sr;

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
    m_data   = std::move( image.m_data );

    image.m_width  = 0u;
    image.m_height = 0u;

    return *this;
}

void Image::resize( uint32_t width, uint32_t height )
{
    if ( m_width == width && m_height == height )
        return;

    m_data   = std::make_unique<Color[]>( static_cast<uint64_t>( width ) * height );
    m_width  = width;
    m_height = height;
}

void Image::clear( const Color& color )
{
    Color* p = m_data.get();
    for ( uint32_t i = 0; i < m_width * m_height; ++i )
        p[i] = color;
}


void Image::line( int x0, int y0, int x1, int y1, const Color& color )
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

        if (e2 >= dy)
        {
            if ( x0 == x1 )
                break;

            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            if ( y0 == y1 )
                break;

            err += dx;
            y0 += sy;
        }
    }
}
