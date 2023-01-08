#pragma once

#include "Character.hpp"
#include "Player.hpp"

#include <Math/AABB.hpp>

class Box final
{
public:
    enum class State
    {
        Idle,
        Hit,
        Break
    };

    Box() = default;
    Box( const Character& boxCharacter, int hitPoints, const Math::AABB& collision );
    ~Box() = default;

    /// <summary>
    /// Check to see if the player collides with this box.
    /// </summary>
    bool collides( const Player& player ) const noexcept
    {
        return player.getAABB().intersect( aabb );
    }

    /// <summary>
    /// The player hit the box. Reduce the hit points by 1 and return the remaining hitpoints.
    /// </summary>
    /// <returns></returns>
    int hit()
    {
        if (hitPoints > 0)
        {
            state = State::Hit;

        }
    }

    /// <summary>
    /// Update the box.
    /// </summary>
    /// <param name="deltaTime">The frame time (in seconds).</param>
    void update( float deltaTime );

    /// <summary>
    /// Draw this box to the image.
    /// </summary>
    /// <param name="image"></param>
    void draw( sr::Image& image ) const;

private:
    void setState( State newState );

    Math::AABB aabb;

    State state = State::Idle;

    // How many hits it takes to destroy the box.
    int hitPoints = 0;

    // The box "character"
    const Character* character = nullptr;
};