
#include <Graphics/File.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/Image.hpp>

#include <stb_easy_font.h>

#include <codecvt>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;
using namespace Graphics;

const Font Font::Default {};

// Convert wchar_t string to char string.
static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> stringConverter;

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

        stbtt_InitFont( &fontInfo, fontData.data(), 0 );
        // int x0, y0, x1, y1;
        // stbtt_GetFontBoundingBox( &fontInfo, &x0, &y0, &x1, &y1 );
        // float scale = stbtt_ScaleForPixelHeight( &fontInfo, size );

        // I'm not sure how to calculate the exact size of the pixel buffer needed for the font.
        // This pixel width is very liberal, but better safe than sorry.
        const int pw         = static_cast<int>( std::ceil( static_cast<float>( numChars ) * size ) );
        const int ph         = static_cast<int>( std::ceil( 2.0f * size ) );
        auto      fontBitmap = std::make_unique<unsigned char[]>( static_cast<size_t>( pw ) * ph );

        int numRows = stbtt_BakeFontBitmap( fontData.data(), 0, size, fontBitmap.get(), pw, ph,
                                            static_cast<int>( firstChar ), static_cast<int>( numChars ), bakedChar.get() );

        // Copy the alpha values of the font bitmap to the font image.
        fontImage = std::make_unique<Image>( pw, ph );
        Color* c  = fontImage->data();
        for ( int y = 0; y < ph; ++y )
        {
            for ( int x = 0; x < pw; ++x )
            {
                size_t        i = static_cast<size_t>( y ) * pw + x;
                unsigned char a = fontBitmap[i];

                c[i] = Color { 255, 255, 255, a };
            }
        }

        // Uncomment the next lines to save the font image to disk.
        // This is mostly used for testing the font baking.
        // auto fontTga = std::format( "{}/{}_{}.tga", fontFile.parent_path().string(), fontFile.stem().string(), size );
        // fontImage->save( fontTga );
    }
    else
    {
        std::cerr << "Error reading file: " << fontFile << std::endl;
    }
}

glm::vec2 Font::getSize( std::string_view text ) const noexcept
{
    float width  = 0.0f;
    float height = 0.0f;

    if ( fontImage && bakedChar )
    {
        // TODO: There must be a better way of computing the size of the text using font metrics.
        // But using GetBakedQuad seems like the most obvious (but not optimal) approach.
        Math::AABB  aabb;
        const char* t    = text.data();
        float       xPos = 0.0f;
        float       yPos = 0.0f;
        while ( *t )
        {
            if ( *t >= firstChar && *t < ( firstChar + numChars ) )
            {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad( bakedChar.get(), static_cast<int>( fontImage->getWidth() ), static_cast<int>( fontImage->getHeight() ), *t - firstChar, &xPos, &yPos, &q, 1 );
                aabb.expand( Math::AABB::fromMinMax( { q.x0, q.y0, 0 }, { q.x1, q.y1, 0 } ) );
            }
            else if ( *t == '\n' )
            {
                xPos = 0.0f;
                yPos += size;
            }

            ++t;
        }
        width  = aabb.width();
        height = aabb.height();
    }
    else
    {
        width  = static_cast<float>( stb_easy_font_width( text.data() ) ) * size;
        height = static_cast<float>( stb_easy_font_height( text.data() ) ) * size;
    }

    return { width, height };
}

void Font::drawText( Image& image, std::string_view text, int x, int y, const Color& color ) const
{
    // Convert to wide character strings.
    std::wstring wText = stringConverter.from_bytes( text.data() );
    drawText( image, wText, x, y, color );
}

void Font::drawText( Image& image, std::wstring_view text, int x, int y, const Color& color ) const
{
    if ( fontImage && bakedChar )
    {
        const wchar_t* t    = text.data();
        auto           xPos = static_cast<float>( x );
        auto           yPos = static_cast<float>( y );
        while ( *t )
        {
            if ( *t >= firstChar && *t < ( firstChar + numChars ) )
            {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad( bakedChar.get(), static_cast<int>( fontImage->getWidth() ), static_cast<int>( fontImage->getHeight() ), static_cast<int>( *t - firstChar ), &xPos, &yPos, &q, 1 );

                Vertex v0 { { q.x0, q.y0 }, { q.s0, q.t0 }, color };
                Vertex v1 { { q.x1, q.y0 }, { q.s1, q.t0 }, color };
                Vertex v2 { { q.x1, q.y1 }, { q.s1, q.t1 }, color };
                Vertex v3 { { q.x0, q.y1 }, { q.s0, q.t1 }, color };

                image.drawQuad( v0, v1, v2, v3, *fontImage, AddressMode::Clamp, BlendMode::AlphaBlend );
                // image.drawQuad( { q.x0, q.y0 }, { q.x1, q.y0 }, { q.x1, q.y1 }, { q.x0, q.y1 }, Color::Red, {}, FillMode::WireFrame ); // For debugging glyph quads.
            }
            else if ( *t == '\n' )
            {
                xPos = static_cast<float>( x );
                yPos += size;
            }

            ++t;
        }
    }
    else
    {
        // Basic fonts only support ASCII characters. Try to convert the text to ASCII...
        std::string strText = stringConverter.to_bytes( text.data() );

        std::vector<FontVertex> vertexBuffer( text.length() * 40 );

        const int numQuads = stb_easy_font_print( 0, 0, strText.data(), nullptr, vertexBuffer.data(), static_cast<int>( vertexBuffer.size() * sizeof( FontVertex ) ) );

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
