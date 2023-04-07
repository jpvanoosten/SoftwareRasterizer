#pragma once

#include "Brick.hpp"
#include "Physics.hpp"

#include "Ball.hpp"
#include <Graphics/Image.hpp>

#include <optional>
#include <vector>

class Game;

class Level
{
public:
    /// <summary>
    /// Default level constructor creates an empty level (no bricks).
    /// </summary>
    Level() = default;

    /// <summary>
    /// Create the level given the specific level ID.
    /// </summary>
    /// <param name="game">A reference to the Game instance.</param>
    /// <param name="levelId">The level to create.</param>
    explicit Level( Game& game, int levelId );

    /// <summary>
    /// Updates the bricks animations.
    /// </summary>
    /// <param name="deltaTime">The elapsed time (in seconds).</param>
    void update( float deltaTime );

    /// <summary>
    /// Draw the bricks for this level.
    /// </summary>
    /// <param name="image">The image to draw the bricks to.</param>
    void draw( Graphics::Image& image ) const;

    /// <summary>
    /// Play the "hit" animation on all of the bricks in the scene.
    /// </summary>
    void animateBricks();

    /// <summary>
    /// Check collision with the ball and the bricks.
    /// </summary>
    /// <param name="ball">The ball to check for collision with.</param>
    /// <returns>An optional HitInfo structure that contains the collision information, otherwise a disengaged optional value if no collision occurred.</returns>
    std::optional<Physics::HitInfo> checkCollision( const Ball& ball );

private:
    // Need a pointer to the game to update the points when a brick is destroyed.
    Game* pGame = nullptr;

    std::vector<Brick> bricks;
};