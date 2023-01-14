#pragma once

#include <Image.hpp>
#include <SpriteAnim.hpp>

#include <Math/Transform2D.hpp>

#include <map>
#include <string>

class Character
{
public:
    Character() = default;
    virtual ~Character() = default;

    Character( const Character& copy );
    Character( Character&& other ) noexcept;

    Character& operator=( const Character& copy );
    Character& operator=( Character&& other ) noexcept;

    /// <summary>
    /// Add a sprite animation to the character.
    /// Note: The character owns the sprite animations so it is advised to move the sprite animations
    /// to the character using `std::move`.
    /// </summary>
    /// <param name="animName">The name of the animation clip.</param>
    /// <param name="anim">The sprite animation.</param>
    void addAnimation( std::string_view animName, sr::SpriteAnim anim );

    /// <summary>
    /// Set the current animation frame.
    /// </summary>
    /// <param name="animName">The animation to set to the currently playing animation for this character.</param>
    void setAnimation( std::string_view animName );

    /// <summary>
    /// Update the character's animation frame.
    /// </summary>
    /// <param name="deltaTime">The elapsed time (in seconds).</param>
    virtual void update( float deltaTime );

    /// <summary>
    /// Draw the character to the image.
    /// </summary>
    /// <param name="image">The image to render the character sprite to.</param>
    /// <param name="transform">The transform to apply.</param>
    void draw( sr::Image& image, const Math::Transform2D& transform ) const;

private:
    // A map of animation names to SpriteAnim(s)
    using AnimMap = std::map<std::string, sr::SpriteAnim>;

    AnimMap anims;
    AnimMap::iterator currentAnim = anims.end();
};

