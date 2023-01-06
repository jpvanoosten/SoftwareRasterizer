#include <SpriteSheet.hpp>

using namespace sr;

SpriteSheet::SpriteSheet( const std::filesystem::path& fileName, std::optional<uint32_t> spriteWidth, std::optional<uint32_t> spriteHeight, const BlendMode& blendMode )
: image { Image::fromFile( fileName ) }
, blendMode { blendMode }
{
    if ( !image )
        return;

    if ( !spriteWidth )
        spriteWidth = image.getWidth();

    if ( !spriteHeight )
        spriteHeight = image.getHeight();

    // Make sure the sprite width/height are evenly divisible by the image width/height.
    assert( image.getWidth() % *spriteWidth == 0 );
    assert( image.getHeight() % *spriteHeight == 0 );

    columns = image.getWidth() / *spriteWidth;
    rows    = image.getHeight() / *spriteHeight;

    spriteRects.reserve( static_cast<size_t>( columns ) * rows );

    const uint32_t w = image.getWidth() / columns;
    const uint32_t h = image.getHeight() / rows;

    for ( uint32_t i = 0; i < rows; ++i )
    {
        for ( uint32_t j = 0; j < columns; ++j )
        {
            spriteRects.emplace_back( j * w, i * h, w, h );
        }
    }

    init();
}

SpriteSheet::SpriteSheet( const std::filesystem::path& fileName, std::span<const Math::RectI> rects, const BlendMode& blendMode )
: image { Image::fromFile( fileName ) }
, blendMode { blendMode }
, spriteRects { rects.begin(), rects.end() }
, columns { static_cast<uint32_t>( rects.size() ) }
, rows { 1u }
{
    init();
}

SpriteSheet::SpriteSheet( const SpriteSheet& copy )
: image { copy.image }
, blendMode { copy.blendMode }
, spriteRects { copy.spriteRects }
, columns { copy.columns }
, rows { copy.rows }
{
    init();
}

SpriteSheet::SpriteSheet( SpriteSheet&& other ) noexcept
: image { std::move( other.image ) }
, blendMode { other.blendMode }
, spriteRects { std::move( other.spriteRects ) }
, columns { other.columns }
, rows { other.rows }
{
    init();

    other.rows    = 0u;
    other.columns = 0u;
    other.sprites.clear();
}

SpriteSheet& SpriteSheet::operator=( const SpriteSheet& copy )
{
    if ( &copy == this )
        return *this;

    image       = copy.image;
    blendMode   = copy.blendMode;
    spriteRects = copy.spriteRects;
    columns     = copy.columns;
    rows        = copy.rows;

    init();

    return *this;
}

SpriteSheet& SpriteSheet::operator=( SpriteSheet&& other ) noexcept
{
    if ( &other == this )
        return *this;

    image       = std::move( other.image );
    blendMode   = other.blendMode;
    spriteRects = std::move( other.spriteRects );
    columns     = other.columns;
    rows        = other.rows;

    init();

    other.columns = 0u;
    other.rows    = 0u;
    other.sprites.clear();

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
    return operator[]( i* columns + j );
}

SpriteSheet SpriteSheet::fromGrid( const std::filesystem::path& fileName, uint32_t columns, uint32_t rows, const BlendMode& blendMode )
{
    Image image = Image::fromFile( fileName );

    // Make sure the image can be split up evenly by the number of sprites.
    assert( image.getWidth() % columns == 0 );
    assert( image.getHeight() % rows == 0 );

    std::vector<Math::RectI> spriteRects;
    spriteRects.reserve( static_cast<size_t>( columns ) * rows );

    const uint32_t w = image.getWidth() / columns;
    const uint32_t h = image.getHeight() / rows;

    for ( uint32_t i = 0; i < rows; ++i )
    {
        for ( uint32_t j = 0; j < columns; ++j )
        {
            spriteRects.emplace_back( j * w, i * h, w, h );
        }
    }

    SpriteSheet sheet;
    sheet.image       = std::move( image );
    sheet.blendMode   = blendMode;
    sheet.spriteRects = std::move( spriteRects );
    sheet.columns     = columns;
    sheet.rows        = rows;

    sheet.init();

    return sheet;
}

void SpriteSheet::init()
{
    sprites.clear();
    sprites.reserve( spriteRects.size() );

    for ( const auto& rect: spriteRects )
    {
        sprites.emplace_back( image, rect, blendMode );
    }
}
