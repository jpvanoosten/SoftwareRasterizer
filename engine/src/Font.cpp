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
        fontData  = File::readFile<unsigned char>( fontFile, std::ios::binary );

        // Calculate the size of the pixel buffer for the font.
        const int pw         = static_cast<int>( std::ceil( size ) ) * static_cast<int>( numChars );
        const int ph         = static_cast<int>( std::ceil( size ) );
        auto      fontBitmap = std::make_unique<unsigned char[]>( static_cast<size_t>( pw ) * ph );

        int numRows = stbtt_BakeFontBitmap( fontData.data(), 0, size, fontBitmap.get(), pw, ph,
                                            static_cast<int>( firstChar ), static_cast<int>( numChars ), bakedChar.get() );

        // Copy the alpha values of the font bitmap to the font image.
        fontImage = std::make_unique<Image>( pw, ph );
        Color* c  = fontImage->data();
        for (int y = 0; y < ph; ++y)
        {
            for (int x = 0; x < pw; ++x)
            {
                c[y * pw + x] = Color( 255, 255, 255, fontBitmap[y * pw + x] );
            }
        }

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
    if ( fontImage && bakedChar )
    {
        //Vertex v0 { { x, y }, { 0, 0 }, color };
        //Vertex v1 { { x + fontImage->getWidth(), y }, { 1, 0 }, color };
        //Vertex v2 { { x + fontImage->getWidth(), y + fontImage->getHeight() }, { 1, 1 }, color };
        //Vertex v3 { { x, y + fontImage->getHeight() }, { 0, 1 }, color };

        //image.drawQuad( v0, v1, v2, v3, *fontImage, BlendMode::AlphaBlend );
        //image.drawQuad( { x, y }, { x + fontImage->getWidth(), y }, { x + fontImage->getWidth(), y + fontImage->getHeight() }, { x, y + fontImage->getHeight() }, Color::Red, {}, FillMode::WireFrame );
        //return;

        const char* t    = text.data();
        auto        xPos = static_cast<float>( x );
        auto        yPos = static_cast<float>( y );
        while ( *t )
        {
            if ( *t >= firstChar && *t < ( firstChar + numChars ) )
            {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad( bakedChar.get(), static_cast<int>( fontImage->getWidth() ), static_cast<int>( fontImage->getHeight() ), *t - firstChar, &xPos, &yPos, &q, 1 );

                Vertex v0 { { q.x0, q.y0 }, { q.s0, q.t0 }, color };
                Vertex v1 { { q.x1, q.y0 }, { q.s1, q.t0 }, color };
                Vertex v2 { { q.x1, q.y1 }, { q.s1, q.t1 }, color };
                Vertex v3 { { q.x0, q.y1 }, { q.s0, q.t1 }, color };

                image.drawQuad( v0, v1, v2, v3, *fontImage, BlendMode::AlphaBlend );
                //image.drawQuad( { q.x0, q.y0 }, { q.x1, q.y0 }, { q.x1, q.y1 }, { q.x0, q.y1 }, Color::Red, {}, FillMode::WireFrame );
            }
            ++t;
        }
    }
    else
    {
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
}
