#pragma once

#include "Config.hpp"
#include "Image.hpp"
#include "SpriteSheet.hpp"

#include <filesystem>

namespace Graphics
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
    /// <param name="columns">The number of columns in the tile map.</param>
    /// <param name="rows">The number of rows in the tile map.</param>
    TileMap( std::shared_ptr<SpriteSheet> spriteSheet, uint32_t columns, uint32_t rows );
    
    /// <summary>
    /// Get the sprite ID at the i^th row and the j^th column in the tile map.
    /// Note: The top-left tile is at (0, 0) and the bottom-right tile is at (rows - 1, columns - 1).
    /// </summary>
    /// <param name="i">The row of the tile in the tile map.</param>
    /// <param name="j">The column of the tile in the tile map.</param>
    /// <returns>
    /// The sprite ID of the sprite in the sprite sheet at the specified grid coordinates.
    /// Returns -1 if there isn't a sprite at the specified grid coordinates.
    /// </returns>
    int operator()( size_t i, size_t j ) const noexcept;

    /// <summary>
    /// Get a reference to a sprite ID at the i^th row and the j^th column in the tile map.
    /// Note: The top-left tile is at (0, 0) and the bottom-right tile is at (rows - 1, columns - 1).
    /// </summary>
    /// <param name="i">The x-coordinate of the tile in the tile map. Must be in the range [0 ... rows - 1]</param>
    /// <param name="j">The y-coordinate of the tile in the tile map. Must be in the range [0 ... columns - 1]</param>
    /// <returns>The sprite ID of the sprite in the sprite sheet at the specified grid coordinate.</returns>
    int& operator()( size_t i, size_t j ) noexcept;

    /// <summary>
    /// Clear the sprite grid (set all sprite values to -1).
    /// </summary>
    void clear();

    /// <summary>
    /// Get the number of columns in the tile map.
    /// </summary>
    /// <returns>The number columns tile map.</returns>
    uint32_t getColumns() const noexcept
    {
        return columns;
    }

    /// <summary>
    /// Get the number of rows in the tile map.
    /// </summary>
    /// <returns>The number of rows in the tile map.</returns>
    uint32_t getRows() const noexcept
    {
        return rows;
    }

    /// <summary>
    /// Get the width of a sprite in the sprite sheet.
    /// </summary>
    /// <returns>The width (in pixels) of a sprite in the sprite sheet.</returns>
    uint32_t getSpriteWidth() const noexcept
    {
        if ( spriteSheet )
            return spriteSheet->getSpriteWidth();

        return 0u;
    }

    /// <summary>
    /// Get the height of a sprite in the sprite sheet.
    /// </summary>
    /// <returns>The height (in pixels) of a sprite in the sprite sheet.</returns>
    uint32_t getSpriteHeight() const noexcept
    {
        if ( spriteSheet )
            return spriteSheet->getSpriteHeight();

        return 0u;
    }

    /// <summary>
    /// Get the sprite sheet associated with this tile map.
    /// </summary>
    /// <returns>A reference to the sprite sheet.</returns>
    std::shared_ptr<SpriteSheet> getSpriteSheet() const noexcept
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
    // The number of columns in the tile map.
    uint32_t columns = 0u;
    // The number of rows in the tile map.
    uint32_t rows = 0u;

    // The sprite sheet to use for drawing the tilemap.
    std::shared_ptr<SpriteSheet> spriteSheet;
    std::vector<int> spriteGrid;
};
}  // namespace Graphics
