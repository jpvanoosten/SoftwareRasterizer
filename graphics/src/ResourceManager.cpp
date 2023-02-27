#include <Graphics/ResourceManager.hpp>

#include <unordered_map>

using namespace Graphics;

// Image store.
std::unordered_map<std::filesystem::path, std::shared_ptr<Image>> g_ImageMap;

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

std::shared_ptr<SpriteSheet> ResourceManager::loadSpriteSheet( const std::filesystem::path& filePath, std::span<const Math::RectI> spriteRects, const BlendMode& blendMode )
{
    return std::make_shared<SpriteSheet>( filePath, spriteRects, blendMode );
}

void ResourceManager::clear()
{
    g_ImageMap.clear();
}
