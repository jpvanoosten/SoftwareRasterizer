#pragma once

#include "Ball.hpp"
#include "Bullet.hpp"
#include "Field.hpp"
#include "Level.hpp"
#include "PowerUp.hpp"
#include "State.hpp"
#include "Vaus.hpp"

#include <array>
#include <random>

class Game;

class PlayState : public State
{
public:
    enum class State
    {
        Ready,
        Appear,
        Start,
        Playing,
        Dead,
        GameOver,
    };

    PlayState( Game& game );

    void update( float deltaTime ) override;
    void draw( Graphics::Image& image ) override;

    void  setState( State newState );
    State getState() const noexcept;

    int getCurrentLevel() const noexcept
    {
        return levelId;
    }

    // Spawn a random power up.
    void spawnPowerUp( const glm::vec2& pos );

private:
    void startState( State newState );
    void endState( State oldState );

    void doReady( float deltaTime );
    void doAppear( float deltaTime );
    void doStart( float deltaTime );
    void doPlaying( float deltaTime );
    void doDead( float deltaTime );

    // Check collisions with the ball.
    void checkCollisions( Ball& ball );
    // Check collisions with a bullet.
    void checkCollisions( Bullet& bullet );
    // Check collision with a power-up.
    // Returns true if the power-up should be destroyed.
    bool checkCollisions( const PowerUp& powerUp );

    void drawText( Graphics::Image& image, std::string_view text, int x, int y );

    Game& game;

    State state = State::Dead;

    Field                 field;
    std::vector<Ball>     balls;  // There can be many balls on the screen at once.
    Vaus                  vaus;
    Level                 level;
    std::array<Bullet, 2> bullets;
    std::vector<PowerUp>  powerUps;

    // Timer for firing bullets.
    float bulletTimeout = 0.0f;

    // Size of the play area.
    int screenWidth;
    int screenHeight;

    // Number of lives.
    int numLives[2] = { 3, 3 };
    int currentPlayer = 0;
    // The current level.
    int levelId = 0;

    float time = 0.0f;

    static inline const float ballSpeed = 200.0f;  // pixels per second

    std::bernoulli_distribution     powerUpChance { 0.2 };     // Chance of spawning a power-up when hitting a brick.
    std::uniform_int_distribution<> powerUpTypeDist { 1, 7 };  // Random type of power-up to create.
    std::minstd_rand                rng;                       // Random number generator.
};
