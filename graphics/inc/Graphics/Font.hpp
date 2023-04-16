#pragma once

#include "Color.hpp"
#include "Config.hpp"

#include <glm/vec2.hpp>
#include <stb_truetype.h>

#include <filesystem>

namespace Graphics
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
    /// <param name="size">(optional) The size of the font (in pixels) to generate. Default: 12</param>
    /// <param name="firstChar">(optional) The first character in the font texture. Default: ' '.</param>
    /// <param name="numChars">(optional) The number of characters in the font texture. Default: 96.</param>
    Font( const std::filesystem::path& fontFile, float size = 12.0f, uint32_t firstChar = 32u, uint32_t numChars = 96u);

    /// <summary>
    /// Get the size of the area needed to render the given text using this font.
    /// </summary>
    /// <param name="text">The text to write.</param>
    /// <returns>The size of the rectangle needed to render this font.</returns>
    glm::vec2 getSize( std::string_view text ) const noexcept;

    float getFontSize() const noexcept
    {
        return size;
    }

    // Font's can't be copied or moved (yet).
    Font( const Font& font ) = delete;
    Font( Font&& font )      = delete;
    ~Font()                  = default;

    Font& operator=( const Font& font )     = delete;
    Font& operator=( Font&& font ) noexcept = delete;

    /// <summary>
    /// Default font uses stb_easy_font for quad font rendering.
    /// </summary>
    static const Font Default;

private:
    friend class Image;

    void drawText( Image& image, std::wstring_view text, int x, int y, const Color& color ) const;
    void drawText( Image& image, std::string_view text, int x, int y, const Color& color ) const;

    // The font size.
    float size;
    uint32_t firstChar = 0;
    uint32_t numChars = 0;

    stbtt_fontinfo                     fontInfo;
    std::unique_ptr<Image>             fontImage;
    std::unique_ptr<stbtt_bakedchar[]> bakedChar;
    std::vector<unsigned char>         fontData;
};
}  // namespace Graphics
