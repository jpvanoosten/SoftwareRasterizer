#include <Background.hpp>

#include <Graphics/ResourceManager.hpp>

using namespace Graphics;

Background::Background( const std::filesystem::path& filePath, float scale, const glm::vec2& scrollDirection, float scrollSpeed )
: backgroundImage { Graphics::ResourceManager::loadImage( filePath ) }
, scrollDirection { scrollDirection }
, scrollSpeed { scrollSpeed }
, scale { scale }
{}

void Background::update( const Graphics::Timer& timer )
{
    textureOffset -= scrollDirection * scrollSpeed * static_cast<float>( timer.elapsedSeconds() );
}

void Background::draw( Graphics::Image& dst ) const
{
    if ( !backgroundImage )
        return;

    // Cover the entire image with the tiled background image.
    const float scaleWidth   = static_cast<float>( dst.getWidth() ) / static_cast<float>( backgroundImage->getWidth() ) / scale;
    const float scaledHeight = static_cast<float>( dst.getHeight() ) / static_cast<float>( backgroundImage->getHeight() ) / scale;

    // Quad vertices.
    const Vertex2D v[] = {
        Vertex2D { { 0, 0 }, { textureOffset.x, textureOffset.y } },                                                         // Top-left
        Vertex2D { { dst.getWidth(), 0 }, { scaleWidth + textureOffset.x, textureOffset.y } },                               // Top-right
        Vertex2D { { dst.getWidth(), dst.getHeight() }, { scaleWidth + textureOffset.x, scaledHeight + textureOffset.y } },  // Bottom-right
        Vertex2D { { 0, dst.getHeight() }, { textureOffset.x, scaledHeight + textureOffset.y } }                             // Bottom-left
    };

    dst.drawQuad( v[0], v[1], v[2], v[3], *backgroundImage, AddressMode::Wrap );
}
