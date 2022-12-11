#include <Font.hpp>
#include <Image.hpp>

#include <stb_easy_font.h>
#include <stb_truetype.h>

#include <vector>

using namespace sr;

const Font Font::Default {};

struct FontVertex
{
    float   x, y, z;
    uint8_t color[4];
};

Font::Font(float size)
    : size{size}
{}

Font::Font( const std::filesystem::path& fontFile, float size )
    : size{size}
{
    // TODO: Load a font
}

glm::ivec2 Font::getSize( std::string_view text ) const noexcept
{
    int width  = stb_easy_font_width( text.data() );
    int height = stb_easy_font_height( text.data() );

    return { width, height };
}

void Font::drawText( Image& image, std::string_view text, int x, int y, const Color& color ) const
{
    // TODO: Figure out how many segments are used by the most complex letters.
    std::vector<FontVertex> vertexBuffer( text.length() * 40 );

    const int numQuads = stb_easy_font_print( static_cast<float>( x ), static_cast<float>( y ), text.data(), nullptr, vertexBuffer.data(), static_cast<int>( vertexBuffer.size() * sizeof( FontVertex ) ) );

    // Scale the quads.
    for (auto& v : vertexBuffer)
    {
        v.x *= size;
        v.y *= size;
        v.z *= size;
    }

    for (int i = 0; i < numQuads; ++i)
    {
        const FontVertex& v0 = vertexBuffer[i * 4 + 0];
        const FontVertex& v1 = vertexBuffer[i * 4 + 1];
        const FontVertex& v2 = vertexBuffer[i * 4 + 2];
        const FontVertex& v3 = vertexBuffer[i * 4 + 3];

        image.drawQuad( { v0.x, v0.y }, { v1.x, v1.y }, { v2.x, v2.y }, { v3.x, v3.y }, color, {}, FillMode::Solid );
    }
}
