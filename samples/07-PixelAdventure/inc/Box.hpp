#pragma once

#include "Character.hpp"

#include <Math/AABB.hpp>

class Box : public Character
{
public:
    enum class State
    {
        Idle,
        Hit,
        Break
    };

    Box() = default;
    Box( int hitPoints );
    Box( const Box& copy );
    Box( Box&& other ) noexcept;
    ~Box() override = default;

    Box& operator=( const Box& copy );
    Box& operator=( Box&& other ) noexcept;

    void setPosition( const glm::vec2& position );

    const Math::AABB& getAABB() const noexcept
    {
        return aabb;
    }

    /// <summary>
    /// The player hit the box. Reduce the hit points by 1 and return the remaining hitpoints.
    /// </summary>
    /// <returns></returns>
    void hit()
    {
        setState( State::Hit );
    }

    int getHitPoints() const noexcept
    {
        return hitPoints;
    }

    /// <summary>
    /// Update the box.
    /// </summary>
    /// <param name="deltaTime">The frame time (in seconds).</param>
    void update( float deltaTime ) override;

    /// <summary>
    /// Draw this box to the image.
    /// </summary>
    /// <param name="image"></param>
    void draw( sr::Image& image ) const;

    /// <summary>
    /// Set the state of the box.
    /// </summary>
    /// <param name="newState">The new state to set the box to.</param>
    void setState( State newState );

    /// <summary>
    /// Get the current state of the box.
    /// </summary>
    /// <returns>The current box state.</returns>
    State getState() const noexcept
    {
        return state;
    }

private:
    void startState( State newState );
    void endState( State oldState );

    void updateIdle( float deltaTime );
    void updateHit( float deltaTime );
    void updateBreak( float deltaTime );

    Math::AABB        aabb;
    Math::Transform2D transform;

    State state = State::Idle;

    // How many hits it takes to destroy the box.
    int hitPoints = 0;
};