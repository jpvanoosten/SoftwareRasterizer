#include <Graphics/ResourceManager.hpp>

#include <functional> // std::hash
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

// This is stolen from boost.
template<std::size_t Bits>
struct hash_mix_impl;

template<>
struct hash_mix_impl<64>
{
    static std::uint64_t mix( std::uint64_t x )
    {
        constexpr std::uint64_t m = ( static_cast<std::uint64_t>( 0xe9846af ) << 32 ) + 0x9b1a615d;

        x ^= x >> 32;
        x *= m;
        x ^= x >> 32;
        x *= m;
        x ^= x >> 28;

        return x;
    }
};

template<>
struct hash_mix_impl<32>
{
    static std::uint32_t mix( std::uint32_t x )
    {
        constexpr std::uint32_t m1  = 0x21f0aaad;
        constexpr std::uint32_t m2 = 0x735a2d97;

        x ^= x >> 16;
        x *= m1;
        x ^= x >> 15;
        x *= m2;
        x ^= x >> 15;

        return x;
    }
};

inline std::size_t hash_mix( std::size_t v )
{
    return hash_mix_impl<sizeof( std::size_t ) * CHAR_BIT>::mix( v );
}

template<typename T>
void hash_combine( std::size_t& seed, const T& v )
{
    seed = hash_mix( seed + 0x9e3779b9 + std::hash<T>()( v ) );
}

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
    g_FontMap.clear();
}
