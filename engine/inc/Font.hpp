#pragma once

#include "Config.hpp"
#include "Vertex.hpp"

#include <glm/vec2.hpp>

#include <filesystem>

namespace sr
{
class Image;

class SR_API Font
{
public:
    /// <summary>
    /// Create a default font.
    /// Uses stb_easy_font to generate quad-based fonts.
    /// </summary>
    /// <param name="size">The size of the font (in pixels) to generate.</param>
    explicit Font( float size = 1.5f );

    /// <summary>
    /// Load a font from a font file.
    /// </summary>
    /// <param name="fontFile">The TrueType font to load.</param>
    /// <param name="size">The size of the font (in pixels) to generate.</param>
    Font( const std::filesystem::path& fontFile, float size = 12.0f);

    /// <summary>
    /// Get the size of the area needed to render the given text using this font.
    /// </summary>
    /// <param name="text">The text to write.</param>
    /// <returns>The size of the are needed to render this font.</returns>
    glm::ivec2 getSize( std::string_view text ) const noexcept;

    float getFontSize() const noexcept
    {
        return size;
    }

    /// <summary>
    /// Default font uses stb_easy_font for quad font rendering.
    /// </summary>
    static const Font Default;

private:
    // The font size.
    float size;

    friend class Image;

    void drawText( Image& image, std::string_view text, int x, int y, const Color& color ) const;

};
}
