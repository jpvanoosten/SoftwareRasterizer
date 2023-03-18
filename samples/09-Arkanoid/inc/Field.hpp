#pragma once

#include <Graphics/SpriteAnim.hpp>
#include <Graphics/SpriteSheet.hpp>

#include <memory>

class Field
{
public:
    Field();

    explicit Field( std::shared_ptr<Graphics::SpriteSheet> fieldSprites );

    void setField( int field );

    void update( float deltaTime );

    void draw( Graphics::Image& image );

private:
    static inline constexpr float FPS = 6.0f;

    // The current field to render.
    int field = 0;

    std::shared_ptr<Graphics::SpriteSheet> fieldSprites;
    Graphics::SpriteAnim                   exitAnim;
    Graphics::SpriteAnim                   leftEnemyGate;
    Graphics::SpriteAnim                   rightEnemyGate;
};