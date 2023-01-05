#include <SpriteAnim.hpp>

using namespace sr;

SpriteAnim::SpriteAnim( const std::filesystem::path& fileName, uint32_t fps, std::optional<uint32_t> spriteWidth, std::optional<uint32_t> spriteHeight, const BlendMode& blendMode )
: spriteSheet( fileName, spriteWidth, spriteHeight, blendMode )
, frameRate { fps }
{}

SpriteAnim::operator const Sprite&() const noexcept
{
    return at( time );
}

const Sprite& SpriteAnim::operator[]( size_t i ) const noexcept {
    const size_t frame = i % spriteSheet.getNumSprites();
    return spriteSheet[frame];
}

const Sprite& SpriteAnim::at( float _time ) const noexcept
{
    // Compute the current animation frame based on the animation time and the frame rate.
    const uint32_t frame = static_cast<uint32_t>( _time * static_cast<float>( frameRate ) ) % spriteSheet.getNumSprites();
    return spriteSheet[frame];
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
    if (spriteSheet.getNumSprites() > 0)
        return static_cast<float>( spriteSheet.getNumSprites() ) / static_cast<float>( frameRate );

    return 0.0f;
}

bool SpriteAnim::isDone() const noexcept
{
    return time > getDuration();
}
