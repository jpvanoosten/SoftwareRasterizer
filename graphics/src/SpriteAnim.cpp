#include <Graphics/SpriteAnim.hpp>

#include <numeric> // for std::iota.

using namespace Graphics;

SpriteAnim::SpriteAnim( std::shared_ptr<SpriteSheet> _spriteSheet, float fps, std::span<const int> _frames )
: spriteSheet { std::move( _spriteSheet ) }
, frameRate { fps }
{
    if ( _frames.empty() )
    {
        if (spriteSheet)
        {
            frames.resize( spriteSheet->getNumSprites() );
            std::iota( frames.begin(), frames.end(), 0 );
        }
    }
    else
    {
        frames = std::vector( _frames.begin(), _frames.end() );
    }
}

SpriteAnim::operator const Sprite&() const noexcept
{
    return at( time );
}

const Sprite& SpriteAnim::operator[]( size_t i ) const noexcept
{
    if ( spriteSheet )
    {
        const size_t frame = frames[i % frames.size()];
        return ( *spriteSheet )[frame];
    }

    static const Sprite emptySprite;
    return emptySprite;
}

const Sprite& SpriteAnim::at( float _time ) const noexcept
{
    uint32_t frame = 0;

    if ( spriteSheet )
        frame = static_cast<uint32_t>( _time * frameRate );

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
    return static_cast<float>( frames.size() ) / frameRate;
}

bool SpriteAnim::isDone() const noexcept
{
    return time > getDuration();
}
