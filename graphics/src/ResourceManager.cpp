#include <Graphics/ResourceManager.hpp>
#include <hash.hpp>

#include <unordered_map>

using namespace Graphics;

/// <summary>
/// A key used to uniquely identify a font.
/// </summary>
struct FontKey
{
    std::filesystem::path fontFile;
    float                 size;
    uint32_t              firstChar;
    uint32_t              numChars;

    bool operator==( const FontKey& other ) const
    {
        return fontFile == other.fontFile && size == other.size && firstChar == other.firstChar && numChars == other.numChars;
    }
};

// Hasher for a FontKey.
template<>
struct std::hash<FontKey>
{
    size_t operator()( const FontKey& key ) const noexcept
    {
        std::size_t seed = 0;

        hash_combine( seed, key.fontFile );
        hash_combine( seed, key.size );
        hash_combine( seed, key.firstChar );
        hash_combine( seed, key.numChars );

        return seed;
    }
};

// Image store.
static std::unordered_map<std::filesystem::path, std::shared_ptr<Image>> g_ImageMap;

// Model store.
static std::unordered_map<std::filesystem::path, std::shared_ptr<Model>> g_ModelMap;

// Font store.
static std::unordered_map<FontKey, std::shared_ptr<Font>> g_FontMap;

std::shared_ptr<Image> ResourceManager::loadImage( const std::filesystem::path& filePath )
{
    const auto iter = g_ImageMap.find( filePath );

    if ( iter == g_ImageMap.end() )
    {
        auto image = std::make_shared<Image>( filePath );

        g_ImageMap[filePath] = image;

        return image;
    }

    return iter->second;
}

std::shared_ptr<SpriteSheet> ResourceManager::loadSpriteSheet( const std::filesystem::path& filePath, std::optional<uint32_t> spriteWidth, std::optional<uint32_t> spriteHeight, uint32_t padding, uint32_t margin, const BlendMode& blendMode )
{
    auto image = loadImage( filePath );
    return std::make_shared<SpriteSheet>( image, spriteWidth, spriteHeight, padding, margin, blendMode );
}

std::shared_ptr<Model> ResourceManager::loadModel( const std::filesystem::path& filePath )
{
    const auto iter = g_ModelMap.find( filePath );

    if (iter == g_ModelMap.end())
    {
        auto model = std::make_shared<Model>( filePath );
        g_ModelMap[filePath] = model;
        return model;
    }

    return iter->second;
}

std::shared_ptr<Font> ResourceManager::loadFont( const std::filesystem::path& fontFile, float size, uint32_t firstChar, uint32_t numChars )
{
    FontKey    key { fontFile, size, firstChar, numChars };
    const auto iter = g_FontMap.find( key );

    if ( iter == g_FontMap.end() )
    {
        auto font = std::make_shared<Font>( fontFile, size, firstChar, numChars );

        g_FontMap[key] = font;

        return font;
    }

    return iter->second;
}


void ResourceManager::clear()
{
    g_ImageMap.clear();
    g_ModelMap.clear();
    g_FontMap.clear();
}
