#include <SpriteAnim.hpp>

using namespace sr;

SpriteAnim::SpriteAnim( std::shared_ptr<SpriteSheet> spriteSheet, uint32_t fps )
: spriteSheet { std::move( spriteSheet ) }
, frameRate { fps }
, time { 0.0f }
{}

SpriteAnim::operator const Sprite&() const noexcept
{
    return at( time );
}

const Sprite& SpriteAnim::operator[]( size_t i ) const noexcept
{
    if ( spriteSheet )
    {
        const size_t frame = i % spriteSheet->getNumSprites();
        return ( *spriteSheet )[frame];
    }

    static const Sprite emptySprite;
    return emptySprite;
}

const Sprite& SpriteAnim::at( float _time ) const noexcept
{
    uint32_t frame = 0;

    if ( spriteSheet && spriteSheet->getNumSprites() > 0 )
        frame = static_cast<uint32_t>( _time * static_cast<float>( frameRate ) ) % spriteSheet->getNumSprites();

    // Will return an empty sprite if spriteSheet is null.
    return operator[]( frame );
}

void SpriteAnim::update( float deltaTime ) noexcept
{
    time += deltaTime;
}

void SpriteAnim::reset() noexcept
{
    time = 0.0f;
}

float SpriteAnim::getDuration() const noexcept
{
    if ( spriteSheet && spriteSheet->getNumSprites() > 0 )
        return static_cast<float>( spriteSheet->getNumSprites() ) / static_cast<float>( frameRate );

    return 0.0f;
}

bool SpriteAnim::isDone() const noexcept
{
    return time > getDuration();
}
