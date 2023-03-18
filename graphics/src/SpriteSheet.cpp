#include <Graphics/ResourceManager.hpp>
#include <Graphics/SpriteSheet.hpp>

using namespace Graphics;

/// <summary>
/// Helper function to compute the number of sprites in a specific dimension of the sprite sheet.
/// </summary>
/// <param name="imageSize">The size of the sprite sheet image.</param>
/// <param name="spriteSize">The size of a single sprite.</param>
/// <param name="padding">The padding between each sprite in the image.</param>
/// <param name="margin">The spacing around the sprite atlas.</param>
/// <returns></returns>
constexpr uint32_t getNumSprites( uint32_t imageSize, uint32_t spriteSize, uint32_t padding, uint32_t margin )
{
    return ( imageSize + padding - 2 * margin ) / ( padding + spriteSize );
}

/// <summary>
/// Helper function to compute the size of a sprite in a specific dimension of the sprite sheet.
/// </summary>
/// <param name="imageSize">The size of the sprite sheet image.</param>
/// <param name="numSprites">The number of sprites.</param>
/// <param name="padding">The padding between each sprite.</param>
/// <param name="margin">The spacing around the sprite atlas.</param>
/// <returns></returns>
constexpr uint32_t getSpriteSize( uint32_t imageSize, uint32_t numSprites, uint32_t padding, uint32_t margin )
{
    return ( imageSize - 2 * margin - ( numSprites - 1 ) * padding ) / numSprites;
}

SpriteSheet::SpriteSheet( const std::filesystem::path& fileName, std::optional<uint32_t> spriteWidth, std::optional<uint32_t> spriteHeight, uint32_t padding, uint32_t margin, const BlendMode& blendMode )
: image { ResourceManager::loadImage( fileName ) }
, blendMode { blendMode }
, padding { padding }
, margin { margin }
{
    if ( !image )
        return;

    if ( !spriteWidth )
        spriteWidth = image->getWidth() - 2 * margin;

    if ( !spriteHeight )
        spriteHeight = image->getHeight() - 2 * margin;

    columns = ::getNumSprites( image->getWidth(), *spriteWidth, padding, margin );
    rows    = ::getNumSprites( image->getHeight(), *spriteHeight, padding, margin );

    initSpriteRects();
    initSprites();
}

SpriteSheet::SpriteSheet( const std::filesystem::path& fileName, std::span<const Math::RectI> rects, const BlendMode& blendMode )
: image { ResourceManager::loadImage( fileName ) }
, blendMode { blendMode }
, spriteRects { rects.begin(), rects.end() }
, columns { static_cast<uint32_t>( rects.size() ) }
, rows { 1u }
{
    initSprites();
}

SpriteSheet::SpriteSheet( std::shared_ptr<Image> _image, std::optional<uint32_t> spriteWidth, std::optional<uint32_t> spriteHeight, uint32_t padding, uint32_t margin, const BlendMode& blendMode )
: image { std::move( _image ) }
, blendMode { blendMode }
, padding { padding }
, margin { margin }
{
    if ( !image )
        return;

    if ( !spriteWidth )
        spriteWidth = image->getWidth() - 2 * margin;

    if ( !spriteHeight )
        spriteHeight = image->getHeight() - 2 * margin;

    columns = ::getNumSprites( image->getWidth(), *spriteWidth, padding, margin );
    rows    = ::getNumSprites( image->getHeight(), *spriteHeight, padding, margin );

    initSpriteRects();
    initSprites();
}

SpriteSheet::SpriteSheet( const SpriteSheet& copy )
: image { copy.image }
, blendMode { copy.blendMode }
, spriteRects { copy.spriteRects }
, columns { copy.columns }
, rows { copy.rows }
, padding { copy.padding }
, margin { copy.margin }
{
    initSprites();
}

SpriteSheet::SpriteSheet( SpriteSheet&& other ) noexcept
: image { std::move( other.image ) }
, blendMode { other.blendMode }
, spriteRects { std::move( other.spriteRects ) }
, columns { other.columns }
, rows { other.rows }
, padding { other.padding }
, margin { other.margin }
{
    initSprites();

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
    padding     = copy.padding;
    margin      = copy.margin;

    initSprites();

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
    padding     = other.padding;
    margin      = other.margin;

    initSprites();

    other.columns = 0u;
    other.rows    = 0u;
    other.padding = 0u;
    other.margin  = 0u;
    other.sprites.clear();

    return *this;
}

const Sprite& SpriteSheet::getSprite( size_t index ) const noexcept
{
    if ( index < sprites.size() )
        return sprites[index];

    static const Sprite emptySprite;
    return emptySprite;
}

const Sprite& SpriteSheet::operator[]( size_t index ) const noexcept
{
    return getSprite( index );
}

const Sprite& SpriteSheet::operator()( size_t i, size_t j ) const noexcept
{
    return getSprite( i * columns + j );
}

std::shared_ptr<SpriteSheet> SpriteSheet::fromGrid( const std::filesystem::path& fileName, uint32_t columns, uint32_t rows, uint32_t padding, uint32_t margin, const BlendMode& blendMode )
{
    std::shared_ptr<Image> image = ResourceManager::loadImage( fileName );

    if ( !image )
        return nullptr;

    std::vector<Math::RectI> spriteRects;
    spriteRects.reserve( static_cast<size_t>( columns ) * rows );

    const uint32_t w = ::getSpriteSize( image->getWidth(), columns, padding, margin );
    const uint32_t h = ::getSpriteSize( image->getHeight(), rows, padding, margin );

    return std::make_shared<SpriteSheet>( image, w, h, padding, margin, blendMode );
}

void SpriteSheet::initSpriteRects()
{
    spriteRects.clear();
    spriteRects.reserve( static_cast<size_t>( columns ) * rows );

    if ( !image )
        return;

    const uint32_t w = ::getSpriteSize( image->getWidth(), columns, padding, margin );
    const uint32_t h = ::getSpriteSize( image->getHeight(), rows, padding, margin );

    for ( uint32_t i = 0; i < rows; ++i )
    {
        for ( uint32_t j = 0; j < columns; ++j )
        {
            spriteRects.emplace_back( margin + j * ( w + padding ), margin + i * ( h + padding ), w, h );
        }
    }
}

void SpriteSheet::initSprites()
{
    sprites.clear();
    sprites.reserve( spriteRects.size() );

    if ( !image )
        return;

    for ( const auto& rect: spriteRects )
    {
        sprites.emplace_back( image, rect, blendMode );
    }
}
