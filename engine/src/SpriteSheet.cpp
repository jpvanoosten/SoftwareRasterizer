#include <SpriteSheet.hpp>

using namespace sr;

SpriteSheet::SpriteSheet( const std::filesystem::path& fileName, uint32_t columns, uint32_t rows, const BlendMode& blendMode )
: image { Image::fromFile( fileName ) }
, columns { columns }
, rows { rows }
{
    // Make sure the image can be split up evenly.
    assert( image.getWidth() % columns == 0 );
    assert( image.getHeight() % rows == 0 );

    sprites.reserve( static_cast<size_t>( columns ) * rows );

    const int w = static_cast<int>( image.getWidth() ) / columns;
    const int h = static_cast<int>( image.getHeight() ) / rows;

    for ( int i = 0; i < rows; ++i )
    {
        for ( int j = 0; j < columns; ++j )
        {
            sprites.emplace_back( image, Math::RectI { j * w, i * h, w, h }, blendMode );
        }
    }
}

SpriteSheet::SpriteSheet( const std::filesystem::path& fileName, std::span<const Math::RectI> rects, const BlendMode& blendMode )
: image { Image::fromFile( fileName ) }
, columns { static_cast<uint32_t>( rects.size() ) }
, rows { 1u }
{
    sprites.reserve( rects.size() );

    for ( auto& rect: rects )
    {
        sprites.emplace_back( image, rect, blendMode );
    }
}

SpriteSheet::SpriteSheet( SpriteSheet&& other ) noexcept
: image { std::move( other.image ) }
, sprites { std::move( other.sprites ) }
, columns { other.columns }
, rows { other.rows }
{
    other.rows    = 0u;
    other.columns = 0u;
}

SpriteSheet& SpriteSheet::operator=( SpriteSheet&& other ) noexcept
{
    image   = std::move( other.image );
    sprites = std::move( other.sprites );
    columns = other.columns;
    rows    = other.rows;

    other.columns = 0u;
    other.rows    = 0u;

    return *this;
}

const Sprite& SpriteSheet::operator[]( size_t index ) const noexcept
{
    if ( index < sprites.size() )
        return sprites[index];

    static const Sprite emptySprite;
    return emptySprite;
}

const Sprite& SpriteSheet::operator()( size_t i, size_t j ) const noexcept
{
    return operator[]( i * columns + j );
}
