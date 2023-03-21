#pragma once

#include <Graphics/SpriteAnim.hpp>
#include <Graphics/SpriteSheet.hpp>

#include <array>
#include <memory>

class Field
{
public:
    Field();

    explicit Field( std::shared_ptr<Graphics::SpriteSheet> fieldSprites );

    void setLevel( int level );

    void setLives( int lives );

    void update( float deltaTime );

    void draw( Graphics::Image& image );

private:
    static inline constexpr float FPS = 6.0f;

    // The current level.
    // The level determines which field and player ship sprite to show.
    int level = 0;
    // Number of player lives.
    int lives = 3;

    std::shared_ptr<Graphics::SpriteSheet> fieldSprites;
    std::array<Graphics::Sprite, 5>        shipSprites;
    Graphics::SpriteAnim                   exitAnim;
    Graphics::SpriteAnim                   leftEnemyGate;
    Graphics::SpriteAnim                   rightEnemyGate;
};