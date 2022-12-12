#include <File.hpp>
#include <Font.hpp>
#include <Image.hpp>

#include <stb_easy_font.h>

#include <iostream>
#include <vector>

namespace fs = std::filesystem;
using namespace sr;

const Font Font::Default {};

struct FontVertex
{
    float   x, y, z;
    uint8_t color[4];
};

Font::Font( float size )
: size { size }
{}

Font::Font( const std::filesystem::path& fontFile, float size, uint32_t firstChar, uint32_t numChars )
: size { size }
, firstChar { firstChar }
, numChars { numChars }
{
    if ( fs::exists( fontFile ) && fs::is_regular_file( fontFile ) )
    {
        bakedChar = std::make_unique<stbtt_bakedchar[]>( numChars );
        fontData  = File::readFile<unsigned char>( fontFile );
        // This should be big enough?
        fontImage = std::make_unique<Image>( static_cast<uint32_t>( size * numChars ), static_cast<uint32_t>( size * numChars ) );

        stbtt_BakeFontBitmap( fontData.data(), 0, size, reinterpret_cast<unsigned char*>( fontImage->data() ),
                              static_cast<int>( fontImage->getWidth() ), static_cast<int>( fontImage->getHeight() ),
                              static_cast<int>( firstChar ), static_cast<int>( numChars ), bakedChar.get() );
    }
    else
    {
        std::cerr << "Error reading file: " << fontFile << std::endl;
    }
}

glm::ivec2 Font::getSize( std::string_view text ) const noexcept
{
    int width  = stb_easy_font_width( text.data() ) * size;
    int height = stb_easy_font_height( text.data() ) * size;

    return { width, height };
}

void Font::drawText( Image& image, std::string_view text, int x, int y, const Color& color ) const
{
    // TODO: Figure out how many segments are used by the most complex letters.
    std::vector<FontVertex> vertexBuffer( text.length() * 40 );

    const int numQuads = stb_easy_font_print( 0, 0, text.data(), nullptr, vertexBuffer.data(), static_cast<int>( vertexBuffer.size() * sizeof( FontVertex ) ) );

    // Scale the quads.
    for ( auto& v: vertexBuffer )
    {
        v.x = v.x * size + static_cast<float>( x );
        v.y = v.y * size + static_cast<float>( y );
        v.z = v.z * size;
    }

    for ( int i = 0; i < numQuads; ++i )
    {
        const FontVertex& v0 = vertexBuffer[i * 4 + 0];
        const FontVertex& v1 = vertexBuffer[i * 4 + 1];
        const FontVertex& v2 = vertexBuffer[i * 4 + 2];
        const FontVertex& v3 = vertexBuffer[i * 4 + 3];

        image.drawQuad( { v0.x, v0.y }, { v1.x, v1.y }, { v2.x, v2.y }, { v3.x, v3.y }, color, {}, FillMode::Solid );
    }
}
