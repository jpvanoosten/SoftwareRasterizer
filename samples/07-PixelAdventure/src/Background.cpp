#include <Background.hpp>

#include <ResourceManager.hpp>

using namespace sr;

Background::Background( const std::filesystem::path& filePath, float scale, const glm::vec2& scrollDirection, float scrollSpeed )
: backgroundImage { sr::ResourceManager::loadImage( filePath ) }
, scrollDirection { scrollDirection }
, scrollSpeed { scrollSpeed }
, scale { scale }
{}

void Background::update( const sr::Timer& timer )
{
    textureOffset -= scrollDirection * scrollSpeed * static_cast<float>( timer.elapsedSeconds() );
}

void Background::draw( sr::Image& dst ) const
{
    if ( !backgroundImage )
        return;

    // Cover the entire image with the tiled background image.
    const float scaleWidth   = static_cast<float>( dst.getWidth() ) / static_cast<float>( backgroundImage->getWidth() ) / scale;
    const float scaledHeight = static_cast<float>( dst.getHeight() ) / static_cast<float>( backgroundImage->getHeight() ) / scale;

    // Quad vertices.
    const Vertex v[] = {
        Vertex { { 0, 0 }, { textureOffset.x, textureOffset.y } },                                                         // Top-left
        Vertex { { dst.getWidth(), 0 }, { scaleWidth + textureOffset.x, textureOffset.y } },                               // Top-right
        Vertex { { dst.getWidth(), dst.getHeight() }, { scaleWidth + textureOffset.x, scaledHeight + textureOffset.y } },  // Bottom-right
        Vertex { { 0, dst.getHeight() }, { textureOffset.x, scaledHeight + textureOffset.y } }                             // Bottom-left
    };

    dst.drawQuad( v[0], v[1], v[2], v[3], *backgroundImage, AddressMode::Wrap );
}
