#include <Transition.hpp>

#include <Graphics/ResourceManager.hpp>

using namespace sr;

Transition::Transition( const std::filesystem::path& fileName )
: transition { ResourceManager::loadImage( fileName )}
, sprite { transition, BlendMode::AlphaBlend }
{
    if ( !transition )
        return;

    for ( int y = 0; y < 5; ++y )
    {
        for ( int x = 0; x < 7; ++x )
        {
            auto& t = transforms.emplace_back( glm::vec2 { x * 80, y * 64 } );
            // Center the anchor on the sprite.
            t.setAnchor( glm::vec2 { transition->getWidth() / 2, transition->getHeight() / 2 } );
        }
    }
}

Transition& Transition::operator=( const Transition& copy )
{
    if ( this == &copy )
        return *this;

    transition = copy.transition;
    sprite     = Sprite { transition, BlendMode::AlphaBlend };
    time       = copy.time;
    transforms = copy.transforms;

    return *this;
}

Transition& Transition::operator=( Transition&& other ) noexcept
{
    if ( this == &other )
        return *this;

    transition = std::move( other.transition );
    sprite     = Sprite { transition, BlendMode::AlphaBlend };
    time       = other.time;
    transforms = std::move( other.transforms );

    return *this;
}

void Transition::setRatio( float ratio )
{
    const float scale = ratio * maxScale;

    for ( auto& transform: transforms )
    {
        transform.setScale( glm::vec2 { scale } );
    }
}

void Transition::draw( sr::Image& image ) const
{
    for ( auto& transform: transforms )
    {
        image.drawSprite( sprite, transform );
    }
}
