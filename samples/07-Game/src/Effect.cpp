#include <Effect.hpp>

Effect::Effect( const sr::SpriteAnim& spriteAnim, const Math::Transform2D& transform )
: spriteAnim{&spriteAnim}
, transform{transform}
{}

void Effect::update( float deltaTime )
{
    time += deltaTime;
}

void Effect::draw( sr::Image& image ) const
{
    if ( spriteAnim )
        image.drawSprite( spriteAnim->at( time ), transform );
}

bool Effect::isDone() const noexcept
{
    if ( spriteAnim )
        return time > spriteAnim->getDuration();

    // Effects without a valid sprite animation are always done.
    return true;
}
