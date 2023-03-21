#include <Graphics/Font.hpp>
#include <Graphics/Image.hpp>
#include <Graphics/Sprite.hpp>
#include <Graphics/Vertex.hpp>

#include <Math/AABB.hpp>
#include <Math/Math.hpp>

#include <stb_image.h>
#include <stb_image_write.h>

#include <algorithm>
#include <iostream>
#include <numbers>
#include <optional>

using namespace Graphics;
using namespace Math;

Image::Image() = default;

Image::Image( const std::filesystem::path& fileName )
{
    int            x, y, n;
    unsigned char* data = stbi_load( fileName.string().c_str(), &x, &y, &n, STBI_rgb_alpha );
    if ( !data )
    {
        std::cerr << "ERROR: Could not load: " << fileName.string() << std::endl;
        return;
    }

    // Convert ARGB
    unsigned char* p = data;
    for ( size_t i = 0; i < x * y; ++i )
    {
        unsigned char c = p[0];
        p[0]            = p[2];
        p[2]            = c;
        p += 4;
    }

    resize( static_cast<uint32_t>( x ), static_cast<uint32_t>( y ) );

    memcpy_s( m_data.get(), static_cast<rsize_t>( m_width ) * m_height * sizeof( Color ), data, static_cast<rsize_t>( m_width ) * m_height * sizeof( Color ) );

    stbi_image_free( data );
}

Image::Image( const Image& copy )
{
    resize( copy.m_width, copy.m_height );
    memcpy_s( data(), static_cast<rsize_t>( m_width ) * m_height * sizeof( Color ), copy.data(), static_cast<rsize_t>( copy.m_width ) * copy.m_height * sizeof( Color ) );
}

Image::Image( Image&& move ) noexcept
: m_width { move.m_width }
, m_height { move.m_height }
, m_AABB { move.m_AABB }
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
    m_AABB   = image.m_AABB;

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

    // Align color buffer to 64-byte boundary for better cache alignment on 64-bit architectures.
    m_data = make_aligned_unique<Color[], 64>( static_cast<uint64_t>( width ) * height );
}

void Image::save( const std::filesystem::path& file ) const
{
    const auto extension = file.extension();

    if ( extension == ".png" )
    {
        stbi_write_png( file.string().c_str(), static_cast<int>( m_width ), static_cast<int>( m_height ), 4, m_data.get(), static_cast<int>( m_width * sizeof( Color ) ) );
    }
    else if ( extension == ".bmp" )
    {
        stbi_write_bmp( file.string().c_str(), static_cast<int>( m_width ), static_cast<int>( m_height ), 4, m_data.get() );
    }
    else if ( extension == ".tga" )
    {
        stbi_write_tga( file.string().c_str(), static_cast<int>( m_width ), static_cast<int>( m_height ), 4, m_data.get() );
    }
    else if ( extension == ".jpg" )
    {
        stbi_write_jpg( file.string().c_str(), static_cast<int>( m_width ), static_cast<int>( m_height ), 4, m_data.get(), 10 );
    }
    else
    {
        std::cerr << "Invalid file type: " << file << std::endl;
    }
}

void Image::clear( const Color& color ) noexcept
{
    Color* p = data();

#pragma omp parallel for
    for ( int i = 0; i < static_cast<int>( m_width * m_height ); ++i )
        p[i] = color;
}

void Image::copy( const Image& srcImage, std::optional<Math::RectI> srcRect, std::optional<Math::RectI> dstRect, const BlendMode& blendMode )
{
    // If the source rectangle is not provided, use the entire source image.
    AABB srcAABB = AABB::fromRect( srcRect ? *srcRect : srcImage.getRect() );
    // If the destination rect is not provided, use the entire source image.
    // I assume that the "expected behaviour" of this method is to copy the source image to the
    // destination image (without scaling) even if that results in clipping of the source image.
    AABB dstAABB = AABB::fromRect( dstRect ? *dstRect : srcImage.getRect() );

    // If the source AABB doesn't intersect with the source image bounds.
    // In other words, the source image rectangle doesn't cover any part of the source image.
    if ( !srcImage.m_AABB.intersect( srcAABB ) )
        return;

    // Clamp the source AABB to the AABB of the source image (to prevent sampling outside of the source image bounds).
    srcAABB.clamp( srcImage.m_AABB );

    // Source width
    const int sW = static_cast<int>( srcAABB.width() );
    // Source height
    const int sH = static_cast<int>( srcAABB.height() );

    // If the destination AABB doesn't intersect with this image bounds...
    // In other words, the destination bounds is completely offscreen.
    if ( !m_AABB.intersect( dstAABB ) )
        return;

    // Destination width
    const int dW = static_cast<int>( dstAABB.width() );
    // Destination height
    const int dH = static_cast<int>( dstAABB.height() );

    // Clamp the dstAABB to the bounds of this image (to prevent writing outside of this image's bounds).
    AABB dstImage = dstAABB.clamped( m_AABB );

    // Clamped image width.
    const int iW = static_cast<int>( dstImage.width() );
    // Clamped image height.
    const int iH = static_cast<int>( dstImage.height() );
    // Clamped image area
    const int iA = iW * iH;

    // Pointer to source image data.
    const Color* src = srcImage.data();
    // Pointer to destination image data.
    Color* dst = data();

#pragma omp parallel for firstprivate( srcAABB, dstAABB, dstImage, sW, sH, dW, dH, iW, iH )
    for ( int i = 0; i < iA; ++i )
    {
        const int x  = i % iW;
        const int y  = i / iW;
        const int dx = x + static_cast<int>( dstImage.min.x );
        const int dy = y + static_cast<int>( dstImage.min.y );
        const int sx = ( x * sW / dW ) + static_cast<int>( srcAABB.min.x );
        const int sy = ( y * sH / dH ) + static_cast<int>( srcAABB.min.y );

        const Color sC = src[sy * srcImage.getWidth() + sx];
        const Color dC = dst[dy * m_width + dx];

        dst[dy * m_width + dx] = blendMode.Blend( sC, dC );
    }
}

void Image::copy( const Image& srcImage, int x, int y )
{
    // Source image coords.
    const int sX = x < 0 ? -x : 0;
    const int sY = y < 0 ? -y : 0;
    const int sW = static_cast<int>( srcImage.getWidth() ) - sX;
    const int sH = static_cast<int>( srcImage.getHeight() ) - sY;

    // Check if source image is offscreen.
    if ( sW <= 0 || sH <= 0 )
        return;

    // Destination coords.
    const int dX = x < 0 ? 0 : x;
    const int dY = y < 0 ? 0 : y;
    const int dW = static_cast<int>( m_width ) - dX;
    const int dH = static_cast<int>( m_height ) - dY;

    // Check if the destination range is offscreen.
    if ( dW <= 0 || dH <= 0 )
        return;

    // The destination copy region is the minimum of the source
    // and destination dimensions.
    const int w = std::min( sW, dW );
    const int h = std::min( sH, dH );

    const uint32_t srcWidth = srcImage.getWidth();
    const Color*   src      = srcImage.data();
    Color*         dst      = data();

#pragma omp parallel for firstprivate( w, h, sX, sY, dX, dY )
    for ( int i = 0; i < h; ++i )
        memcpy_s( dst + ( i + dY ) * m_width + dX, w * sizeof( Color ), src + ( i + sY ) * srcWidth + sX, w * sizeof( Color ) );
}

// Source: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void Image::drawLine( int x0, int y0, int x1, int y1, const Color& color, const BlendMode& blendMode ) noexcept
{
    // Shrink the image AABB by 1 pixel to prevent drawing the line outside of the image bounds.
    if ( !m_AABB.clip( x0, y0, x1, y1 ) )
        return;

    const int dx = std::abs( x1 - x0 );
    const int dy = -std::abs( y1 - y0 );
    const int sx = x0 < x1 ? 1 : -1;
    const int sy = y0 < y1 ? 1 : -1;

    int err = dx + dy;

    while ( true )
    {
        plot<false>( x0, y0, color, blendMode );
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

void Image::drawTriangle( const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const Color& color, const BlendMode& blendMode, FillMode fillMode ) noexcept
{
    // Create an AABB for the triangle.
    AABB aabb = AABB::fromTriangle( { p0, 0 }, { p1, 0 }, { p2, 0 } );

    // Check if the triangle is on screen.
    if ( !m_AABB.intersect( aabb ) )
        return;

    switch ( fillMode )
    {
    case FillMode::WireFrame:
    {
        drawLine( p0, p1, color, blendMode );
        drawLine( p1, p2, color, blendMode );
        drawLine( p2, p0, color, blendMode );
    }
    break;
    case FillMode::Solid:
    {
        // Clamp the triangle AABB to the screen bounds.
        aabb.clamp( m_AABB );

#pragma omp parallel for schedule( dynamic ) firstprivate( aabb )
        for ( int y = static_cast<int>( aabb.min.y ); y <= static_cast<int>( aabb.max.y ); ++y )
        {
            for ( int x = static_cast<int>( aabb.min.x ); x <= static_cast<int>( aabb.max.x ); ++x )
            {
                if ( pointInsideTriangle( { x, y }, p0, p1, p2 ) )
                    plot<false>( x, y, color, blendMode );
            }
        }
    }
    break;
    }
}

void Image::drawQuad( const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const Color& color, const BlendMode& blendMode, FillMode fillMode ) noexcept
{
    AABB aabb = AABB::fromQuad( { p0, 0 }, { p1, 0 }, { p2, 0 }, { p3, 0 } );

    // Check if the triangle is on screen.
    if ( !m_AABB.intersect( aabb ) )
        return;

    switch ( fillMode )
    {
    case FillMode::WireFrame:
    {
        drawLine( p0, p1, color, blendMode );
        drawLine( p1, p2, color, blendMode );
        drawLine( p2, p3, color, blendMode );
        drawLine( p3, p0, color, blendMode );
    }
    break;
    case FillMode::Solid:
    {
        glm::vec2 verts[] = {
            p0, p1, p2, p3
        };

        // Index buffer for the two triangles of the quad.
        const uint32_t indicies[] = {
            0, 1, 3,
            1, 2, 3
        };

        // Clamp to the size of the screen.
        aabb.clamp( m_AABB );

#pragma omp parallel for schedule( dynamic ) firstprivate( aabb, indicies, verts )
        for ( int y = static_cast<int>( aabb.min.y ); y <= static_cast<int>( aabb.max.y ); ++y )
        {
            for ( int x = static_cast<int>( aabb.min.x ); x <= static_cast<int>( aabb.max.x ); ++x )
            {
                for ( uint32_t i = 0; i < std::size( indicies ); i += 3 )
                {
                    const uint32_t i0 = indicies[i + 0];
                    const uint32_t i1 = indicies[i + 1];
                    const uint32_t i2 = indicies[i + 2];

                    glm::vec3 bc = barycentric( verts[i0], verts[i1], verts[i2], { x, y } );
                    if ( barycentricInside( bc ) )
                    {
                        plot<false>( static_cast<uint32_t>( x ), static_cast<uint32_t>( y ), color, blendMode );
                    }
                }
            }
        }
    }
    break;
    }
}

void Image::drawQuad( const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3, const Image& image, AddressMode addressMode, const BlendMode& _blendMode ) noexcept
{
    // Compute an AABB over the sprite quad.
    AABB aabb {
        { v0.position, 0.0f },
        { v1.position, 0.0f },
        { v2.position, 0.0f },
        { v3.position, 0.0f }
    };

    // Check if the AABB of the sprite is on screen.
    if ( !m_AABB.intersect( aabb ) )
        return;

    // Clamp to the size of the screen.
    aabb.clamp( m_AABB );

    Vertex verts[] = {
        v0, v1, v2, v3
    };

    // Index buffer for the two triangles of the quad.
    const uint32_t indicies[] = {
        0, 1, 3,
        1, 2, 3
    };

    const BlendMode blendMode = _blendMode;

#pragma omp parallel for schedule( dynamic ) firstprivate( aabb, indicies, verts, addressMode, blendMode )
    for ( int y = static_cast<int>( aabb.min.y ); y <= static_cast<int>( aabb.max.y ); ++y )
    {
        for ( int x = static_cast<int>( aabb.min.x ); x <= static_cast<int>( aabb.max.x ); ++x )
        {
            for ( uint32_t i = 0; i < std::size( indicies ); i += 3 )
            {
                const uint32_t i0 = indicies[i + 0];
                const uint32_t i1 = indicies[i + 1];
                const uint32_t i2 = indicies[i + 2];

                glm::vec3 bc = barycentric( verts[i0].position, verts[i1].position, verts[i2].position, { x, y } );
                if ( barycentricInside( bc ) )
                {
                    // Compute interpolated UV
                    const glm::vec2 texCoord = verts[i0].texCoord * bc.x + verts[i1].texCoord * bc.y + verts[i2].texCoord * bc.z;
                    const Color     color    = verts[i0].color * bc.x + verts[i1].color * bc.y + verts[i2].color * bc.z;
                    // Sample the texture.
                    const Color c = image.sample( texCoord.x, texCoord.y, addressMode ) * color;
                    // Plot.
                    plot<false>( static_cast<uint32_t>( x ), static_cast<uint32_t>( y ), c, blendMode );
                }
            }
        }
    }
}

void Image::drawAABB( AABB aabb, const Color& color, const BlendMode& blendMode, FillMode fillMode ) noexcept
{
    if ( !m_AABB.intersect( aabb ) )
        return;

    switch ( fillMode )
    {
    case FillMode::WireFrame:
    {
        const glm::ivec2 min     = aabb.min;
        const glm::ivec2 max     = aabb.max;
        const glm::ivec2 verts[] = { { min.x, min.y }, { max.x, min.y }, { max.x, max.y }, { min.x, max.y } };

        for ( int i = 0; i < 4; ++i )
        {
            drawLine( verts[i], verts[( i + 1 ) % 4], color, blendMode );
        }
    }
    break;
    case FillMode::Solid:
    {
        // Clamp to screen bounds.
        aabb.clamp( m_AABB );

#pragma omp parallel for schedule( dynamic ) firstprivate( aabb )
        for ( int y = static_cast<int>( aabb.min.y ); y <= static_cast<int>( aabb.max.y ); ++y )
        {
            for ( int x = static_cast<int>( aabb.min.x ); x <= static_cast<int>( aabb.max.x ); ++x )
            {
                plot<false>( x, y, color, blendMode );
            }
        }
    }
    break;
    }
}

void Image::drawCircle( const Math::Circle& c, const Color& color, const BlendMode& blendMode, FillMode fillMode ) noexcept
{
    if ( !m_AABB.intersect( c ) )
        return;

    for ( int i = 0; i < 64; ++i )
    {
        const float a1 = static_cast<float>( i ) * std::numbers::pi_v<float> / 32.0f;
        const float a2 = static_cast<float>( i + 1 ) * std::numbers::pi_v<float> / 32.0f;

        const glm::vec2 p0 { c.center.x + std::cos( a1 ) * c.radius, c.center.y + std::sin( a1 ) * c.radius };
        const glm::vec2 p1 { c.center.x + std::cos( a2 ) * c.radius, c.center.y + std::sin( a2 ) * c.radius };

        switch ( fillMode )
        {
        case FillMode::WireFrame:
            drawLine( p0, p1, color, blendMode );
            break;
        case FillMode::Solid:
            drawTriangle( p0, p1, c.center, color, blendMode, fillMode );
            break;
        }
    }
}

void Image::drawSprite( const Sprite& sprite, const glm::mat3& matrix ) noexcept
{
    std::shared_ptr<Image> image = sprite.getImage();
    if ( !image )
        return;

    const Color       color     = sprite.getColor();
    const BlendMode   blendMode = sprite.getBlendMode();
    const glm::ivec2& uv        = sprite.getUV();
    const glm::ivec2& size      = sprite.getSize();

    Vertex verts[] = {
        Vertex { { 0, 0 }, { uv.x, uv.y }, color },                                              // Top-left
        Vertex { { size.x - 1, 0 }, { uv.x + size.x - 1, uv.y }, color },                        // Top-right
        Vertex { { size.x - 1, size.y - 1 }, { uv.x + size.x - 1, uv.y + size.y - 1 }, color },  // Bottom-right
        Vertex { { 0, size.y - 1 }, { uv.x, uv.y + size.y - 1 }, color }                         // Bottom-left
    };

    // Transform verts.
    for ( Vertex& v: verts )
    {
        v.position = matrix * glm::vec3 { v.position, 1.0f };
    }

    // Compute an AABB over the sprite quad.
    AABB aabb {
        { verts[0].position, 0.0f },
        { verts[1].position, 0.0f },
        { verts[2].position, 0.0f },
        { verts[3].position, 0.0f }
    };

    // Check if the AABB of the sprite is on screen.
    if ( !m_AABB.intersect( aabb ) )
        return;

    // Clamp to the size of the screen.
    aabb.clamp( m_AABB );

    // Index buffer for the two triangles of the quad.
    const uint32_t indicies[] = {
        0, 1, 3,
        1, 2, 3
    };

#pragma omp parallel for schedule( dynamic ) firstprivate( aabb, indicies, verts, color, blendMode )
    for ( int y = static_cast<int>( aabb.min.y ); y <= static_cast<int>( aabb.max.y ); ++y )
    {
        for ( int x = static_cast<int>( aabb.min.x ); x <= static_cast<int>( aabb.max.x ); ++x )
        {
            for ( uint32_t i = 0; i < std::size( indicies ); i += 3 )
            {
                const uint32_t i0 = indicies[i + 0];
                const uint32_t i1 = indicies[i + 1];
                const uint32_t i2 = indicies[i + 2];

                glm::vec3 bc = barycentric( verts[i0].position, verts[i1].position, verts[i2].position, { x, y } );
                if ( barycentricInside( bc ) )
                {
                    // Compute interpolated UV
                    const glm::ivec2 texCoord = round( verts[i0].texCoord * bc.x + verts[i1].texCoord * bc.y + verts[i2].texCoord * bc.z );
                    // Sample the sprite's texture.
                    const Color c = image->sample( texCoord.x, texCoord.y, AddressMode::Clamp ) * color;
                    // Plot.
                    plot<false>( static_cast<uint32_t>( x ), static_cast<uint32_t>( y ), c, blendMode );
                }
            }
        }
    }
}

void Image::drawSprite( const Sprite& sprite, int x, int y ) noexcept
{
    std::shared_ptr<Image> image = sprite.getImage();
    if ( !image )
        return;

    const Color      color     = sprite.getColor();
    const BlendMode  blendMode = sprite.getBlendMode();
    const glm::ivec2 uv        = sprite.getUV();
    const glm::ivec2 size      = sprite.getSize();

    // Source sprite coords
    const int sX = x < 0 ? -x : 0;
    const int sY = y < 0 ? -y : 0;
    const int sW = size.x - sX;
    const int sH = size.y - sY;

    // Check if the sprite is offscreen.
    if ( sW <= 0 || sH <= 0 )
        return;

    // Destination coords.
    const int dX = x < 0 ? 0 : x;
    const int dY = y < 0 ? 0 : y;
    const int dW = static_cast<int>( m_width ) - dX;
    const int dH = static_cast<int>( m_height ) - dY;

    // Check if the destination region is offscreen.
    if ( dW <= 0 || dH <= 0 )
        return;

    // Source image width.
    const int iW = static_cast<int>( image->getWidth() );

    // The destination copy region is the minimum of the source
    // and destination dimensions.
    const int w = std::min( sW, dW );
    const int h = std::min( sH, dH );
    const int a = w * h;

    const Color* src = image->data();
    Color*       dst = data();

#pragma omp parallel for firstprivate( w, h, a, iW, color, blendMode )
    for ( int i = 0; i < a; ++i )
    {
        const int x  = i % w;
        const int y  = i / w;
        const int dx = dX + x;
        const int dy = dY + y;
        const int sx = ( sX + uv.x ) + x;
        const int sy = ( sY + uv.y ) + y;

        Color dC = dst[dy * m_width + dx];
        Color sC = src[sy * iW + sx] * color;

        dst[dy * m_width + dx] = blendMode.Blend( sC, dC );
    }
}

void Image::drawText( const Font& font, std::string_view text, int x, int y, const Color& color ) noexcept
{
    font.drawText( *this, text, x, y, color );
}

void Image::drawText( const Font& font, std::wstring_view text, int x, int y, const Color& color ) noexcept
{
    font.drawText( *this, text, x, y, color );
}

constexpr int fast_floor( float x ) noexcept
{
    return static_cast<int>( static_cast<double>( x ) + 1073741823.0 ) - 1073741823;
}

constexpr int fast_mod( int x, int y ) noexcept
{
    return x - y * fast_floor( static_cast<float>( x ) / static_cast<float>( y ) );
}

const Color& Image::sample( int u, int v, AddressMode addressMode ) const noexcept
{
    const int w = static_cast<int>( m_width );
    const int h = static_cast<int>( m_height );

    switch ( addressMode )
    {
    case AddressMode::Wrap:
    {
        u = fast_mod( u, w );
        v = fast_mod( v, h );
    }
    break;
    case AddressMode::Mirror:
    {
        u = u / w % 2 == 0 ? fast_mod( u, w ) : ( w - 1 ) - fast_mod( u, w );
        v = v / h % 2 == 0 ? fast_mod( v, h ) : ( h - 1 ) - fast_mod( v, h );
    }
    break;
    case AddressMode::Clamp:
    {
        u = std::clamp( u, 0, w - 1 );
        v = std::clamp( v, 0, h - 1 );
    }
    break;
    }

    assert( u >= 0 && u < w );
    assert( v >= 0 && v < h );

    return m_data[static_cast<uint64_t>( v ) * m_width + u];
}
