#include "Image.hpp"

using namespace sr;

Image::Image( uint32_t width, uint32_t height )
{
    resize( width, height );
}

void Image::resize( uint32_t width, uint32_t height )
{
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
