#pragma once

#include "Character.hpp"

#include <Image.hpp>
#include <Math/Transform2D.hpp>

#include <vector>

class Player
{
public:
    explicit Player( const Math::Transform2D& transform = Math::Transform2D{} );

    /// <summary>
    /// Reset the character (and choose a different character).
    /// </summary>
    void reset();

    /// <summary>
    /// Update the player.
    /// </summary>
    /// <param name="deltaTime">The elapsed time (in seconds).</param>
    void update( float deltaTime ) noexcept;

    void                     setTransform( const Math::Transform2D& transform );
    const Math::Transform2D& getTransform() const noexcept;

    /// <summary>
    /// Draw the player to the image.
    /// </summary>
    /// <param name="image">The image to draw the player to.</param>
    void draw( sr::Image& image ) noexcept;

private:
    using CharacterList = std::vector<Character>;

    /// <summary>
    /// A list of possible character models.
    /// </summary>
    CharacterList           characters;
    CharacterList::iterator currentCharacter = characters.end();

    // The player's transform.
    Math::Transform2D transform;

    /// <summary>
    /// The maximum speed of the player.
    /// </summary>
    float playerSpeed = 150.0f;
};