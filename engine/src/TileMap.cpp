#include <TileMap.hpp>

using namespace sr;

TileMap::TileMap( const SpriteSheet& spriteSheet, uint32_t gridWidth, uint32_t gridHeight )
: gridWidth { gridWidth }
, gridHeight { gridHeight }
, spriteSheet { &spriteSheet }
, spriteGrid( static_cast<size_t>( gridWidth ) * gridHeight, -1 )
{}

TileMap::TileMap( TileMap&& other ) noexcept
: gridWidth{other.gridWidth}
, gridHeight{other.gridHeight}
, spriteSheet{ other.spriteSheet }
, spriteGrid { std::move(other.spriteGrid) }
{
    other.gridWidth  = 0u;
    other.gridHeight = 0u;
    other.spriteSheet = nullptr;
}

TileMap& TileMap::operator=( TileMap&& other ) noexcept
{
    if ( this == &other )
        return *this;

    gridWidth = other.gridWidth;
    gridHeight = other.gridHeight;
    spriteSheet = other.spriteSheet;
    spriteGrid  = std::move( other.spriteGrid );

    other.gridWidth = 0u;
    other.gridHeight = 0u;
    other.spriteSheet = nullptr;

    return *this;
}

int TileMap::operator()( size_t x, size_t y ) const noexcept
{
    if ( x < gridWidth && y < gridHeight )
        return spriteGrid[y * gridWidth + x];

    return -1;
}

int& TileMap::operator()( size_t x, size_t y ) noexcept
{
    assert( x < gridWidth );
    assert( y < gridHeight );

    return spriteGrid[y * gridWidth + x];
}

void TileMap::setSpriteGrid( std::span<int> _spriteGrid )
{
    spriteGrid = std::vector<int>( _spriteGrid.begin(), _spriteGrid.end() );
}

void TileMap::draw( Image& image ) const
{
    if ( !spriteSheet )
        return;

    const int spriteWidth  = static_cast<int>( spriteSheet->getSpriteWidth() );
    const int spriteHeight = static_cast<int>( spriteSheet->getSpriteHeight() );
    const int numSprites   = static_cast<int>( spriteSheet->getNumSprites() );

    int x = 0;
    int y = 0;
    for ( uint32_t i = 0u; i < gridHeight; ++i )
    {
        x = 0;
        for ( uint32_t j = 0; j < gridWidth; ++j )
        {
            const int spriteId = spriteGrid[i * gridWidth + j];
            if ( spriteId >= 0 && spriteId < numSprites )
            {
                image.drawSprite( ( *spriteSheet )[spriteId], x, y );
            }
            x += spriteWidth;
        }
        y += spriteHeight;
    }
}
