#pragma once

#include <Image.hpp>
#include <Timer.hpp>

#include <glm/vec2.hpp>

#include <filesystem>

/// <summary>
///  This class is used to draw a background sprite on the screen.
/// </summary>
class Background final
{
public:
    Background() = default;

    /// <summary>
    /// Create a scrolling background sprite.
    /// </summary>
    /// <param name="filePath">The path the to the background image.</param>
    /// <param name="scale">Scale the background sprite.</param>
    /// <param name="scrollDirection">The direction to scroll the background.</param>
    /// <param name="scrollSpeed">The speed to scroll the background.</param>
    explicit Background( const std::filesystem::path& filePath, float scale = 1.0f, const glm::vec2& scrollDirection = { 0.0f, 1.0f }, float scrollSpeed = 1.0f );

    /// <summary>
    /// Update the scrolling background.
    /// </summary>
    /// <param name="timer">The timer used to update the texture offset.</param>
    void update( const sr::Timer& timer );

    /// <summary>
    /// Draw this background to the destination image.
    /// </summary>
    /// <param name="dst">The destination image to draw the background to.</param>
    void draw( sr::Image& dst ) const;

private:
    std::shared_ptr<sr::Image> backgroundImage;

    glm::vec2 scrollDirection { 0.0f };
    float     scrollSpeed { 0.0f };
    float     scale { 1.0f };
    glm::vec2 textureOffset { 0.0f };
};
