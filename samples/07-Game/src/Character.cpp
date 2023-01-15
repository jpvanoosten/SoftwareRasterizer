#include <Character.hpp>

#include <iostream>

Character::Character( const Character& copy )
: anims { copy.anims }
, currentAnim { anims.end() }
{}

Character::Character( Character&& other ) noexcept
: anims { std::move( other.anims ) }
, currentAnim { anims.end() }
{}

Character& Character::operator=( const Character& copy )
{
    if ( this == &copy )
        return *this;

    anims = copy.anims;
    currentAnim = anims.end();

    return *this;
}

Character& Character::operator=( Character&& other ) noexcept
{
    if ( this == &other )
        return *this;

    anims = std::move( other.anims );
    currentAnim = anims.end();

    return *this;
}

void Character::addAnimation( std::string_view animName, sr::SpriteAnim anim )
{
    anims[std::string( animName )] = std::move( anim );
    currentAnim                    = anims.end();
}

void Character::setAnimation( std::string_view animName )
{
    const auto newAnim = anims.find( std::string( animName ) );

    if ( newAnim == anims.end() )
    {
        std::cerr << "Animation with name: " << animName << " not found in animations.";
    }
    else if ( currentAnim != newAnim )
    {
        currentAnim = newAnim;
        currentAnim->second.reset();
    }
}

const sr::SpriteAnim& Character::getAnimation( std::string_view animName ) const
{
    const auto iter = anims.find( std::string( animName ) );
    if ( iter != anims.end() )
        return iter->second;

    std::cerr << "Animation with name: " << animName << " not found in animations.";

    static sr::SpriteAnim emptyAnim;
    return emptyAnim;
}

void Character::update( float deltaTime )
{
    if ( currentAnim != anims.end() )
    {
        currentAnim->second.update( deltaTime );
    }
}

void Character::draw( sr::Image& image, const Math::Transform2D& transform ) const
{
    if ( currentAnim != anims.end() )
    {
        image.drawSprite( currentAnim->second, transform );
    }
}
