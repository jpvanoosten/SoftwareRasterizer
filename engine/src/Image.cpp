#include "Image.hpp"

using namespace sr;

Image::Image(uint32_t width, uint32_t height)
    : m_width(width)
    , m_height(height)
    , m_data{ std::make_unique<Color[]>(m_width * m_height) }
{}

void Image::clear(const Color& color)
{
    Color* p = m_data.get();
    for (uint32_t i = 0; i < m_width * m_height; ++i)
        p[i] = color;
}
