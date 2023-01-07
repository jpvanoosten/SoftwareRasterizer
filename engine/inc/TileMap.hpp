#pragma once

#include "Config.hpp"
#include "Image.hpp"
#include "SpriteSheet.hpp"

#include <filesystem>

namespace sr
{
class SR_API TileMap final
{
public:
    TileMap() = default;

    /// <summary>
    /// Construct a tile map from a sprite sheet and a grid size.
    /// Note: All tiles are initialized to -1.
    /// </summary>
    /// <param name="spriteSheet">The sprite sheet to use for drawing the tile map.</param>
    /// <param name="gridWidth">The number of tiles in the width of the tile map.</param>
    /// <param name="gridHeight">The number of tiles in the height of the tile map.</param>
    TileMap( SpriteSheet spriteSheet, uint32_t gridWidth, uint32_t gridHeight );

    /// <summary>
    /// Copy a tile map.
    /// </summary>
    /// <param name="copy">The tile map to copy to this one.</param>
    TileMap( const TileMap& copy ) = default;

    /// <summary>
    /// Move a tile map to this one.
    /// </summary>
    /// <param name="other">The tile map to move.</param>
    TileMap( TileMap&& other ) noexcept;

    /// <summary>
    /// Assign another tile map by copy.
    /// </summary>
    /// <param name="copy">The copy to assign to this tile map.</param>
    /// <returns>A reference to this tile map.</returns>
    TileMap& operator=( const TileMap& copy ) = default;

    /// <summary>
    /// Move another tile map this this one.
    /// </summary>
    /// <param name="other">The tile map to move to this one.</param>
    /// <returns>A reference to this tile map.</returns>
    TileMap& operator=( TileMap&& other ) noexcept;

    /// <summary>
    /// Get the sprite ID at a specific X, Y coordinate in the tile map.
    /// Note: The top-left tile is at (0, 0) and the bottom-right tile is at (width - 1, height - 1).
    /// </summary>
    /// <param name="x">The x-coordinate of the tile in the tile map.</param>
    /// <param name="y">The y-coordinate of the tile in the tile map.</param>
    /// <returns>
    /// The sprite ID of the sprite in the sprite sheet at the specified grid coordinates.
    /// Returns -1 if there isn't a sprite at the specified grid coordinates.
    /// </returns>
    int operator()( size_t x, size_t y ) const noexcept;

    /// <summary>
    /// Get a reference to a sprite ID at a specific X, Y coordinate in the tile map.
    /// Note: The top-left tile is at (0, 0) and the bottom-right tile is at (width - 1, height - 1).
    /// </summary>
    /// <param name="x">The x-coordinate of the tile in the tile map. Must be in the range [0 ... width - 1]</param>
    /// <param name="y">The y-coordinate of the tile in the tile map. Must be in the range [0 ... height - 1]</param>
    /// <returns>The sprite ID of the sprite in the sprite sheet at the specified grid coordinate.</returns>
    int& operator()( size_t x, size_t y ) noexcept;

    /// <summary>
    /// Clear the sprite grid (set all sprite values to -1).
    /// </summary>
    void clear();

    /// <summary>
    /// Get the number of tiles in the width of the tile map.
    /// </summary>
    /// <returns>The number of tiles in the width of the tile map.</returns>
    uint32_t getGridWidth() const noexcept
    {
        return gridWidth;
    }

    /// <summary>
    /// Get the number of tiles in the height of the tile map.
    /// </summary>
    /// <returns>The number of tiles in the height of the tile map.</returns>
    uint32_t getGridHeight() const noexcept
    {
        return gridHeight;
    }

    /// <summary>
    /// Get the width of a sprite in the sprite sheet.
    /// </summary>
    /// <returns>The width (in pixels) of a sprite in the sprite sheet.</returns>
    uint32_t getSpriteWidth() const noexcept
    {
        return spriteSheet.getSpriteWidth();
    }

    /// <summary>
    /// Get the height of a sprite in the sprite sheet.
    /// </summary>
    /// <returns>The height (in pixels) of a sprite in the sprite sheet.</returns>
    uint32_t getSpriteHeight() const noexcept
    {
        return spriteSheet.getSpriteHeight();
    }

    /// <summary>
    /// Get the sprite sheet associated with this tile map.
    /// </summary>
    /// <returns>A reference to the sprite sheet.</returns>
    const SpriteSheet& getSpriteSheet() const noexcept
    {
        return spriteSheet;
    }

    /// <summary>
    /// Set the sprite IDs for the sprites in the tile map.
    /// </summary>
    /// <param name="spriteGrid">The IDs of the sprites in the tile map.
    /// Tiles that don't contain a sprite should be set to -1.</param>
    void setSpriteGrid( std::span<const int> spriteGrid );

    /// <summary>
    /// Get the sprite IDs for the sprites in the tile map.
    /// </summary>
    /// <returns></returns>
    const std::vector<int>& getSpriteGrid() const noexcept
    {
        return spriteGrid;
    }

    /// <summary>
    /// Draw this tile map to the image.
    /// </summary>
    /// <param name="image">The image to draw the tile map to.</param>
    void draw( Image& image ) const;

private:
    // The width of the tile map (in tiles).
    uint32_t gridWidth = 0u;
    // The height of the tile map (in tiles)
    uint32_t gridHeight = 0u;

    // The sprite sheet to use for drawing the tilemap.
    SpriteSheet      spriteSheet;
    std::vector<int> spriteGrid;
};
}  // namespace sr
