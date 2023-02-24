#include <Game.hpp>

using namespace Math;

Game::Game( uint32_t screenWidth, uint32_t screenHeight )
: image { screenWidth, screenHeight }
, arial20 { "assets/fonts/arial.ttf", 20 }
, arial24 { "assets/fonts/arial.ttf", 24 }
, backgroundImage { "assets/textures/space_background.jpg" }
{
    ball.setPosition( { static_cast<float>( screenWidth ) / 2.0f, static_cast<float>( screenHeight ) / 2.0f } );
    ball.setVelocity( glm::vec2 { 1, -1 } * 300.0f );
}

void Game::update( double deltaTime )
{
    static double      totalTime = 0.0;
    static uint64_t    frames    = 0;
    static std::string fps       = "FPS: 0";

    ++frames;
    totalTime += deltaTime;
    if ( totalTime > 1.0 )
    {
        fps       = std::format( "FPS: {:.3f}", static_cast<double>( frames ) / totalTime );
        frames    = 0;
        totalTime = 0.0;
    }

    ball.update( static_cast<float>( deltaTime ) );
    checkCollisions( ball );

    image.copy( backgroundImage, 0, 0 );

    ball.draw( image );

    // Draw an FPS counter in the corner of the screen.
    image.drawText( arial20, 6, 20, fps, sr::Color::Black );
    image.drawText( arial20, 4, 18, fps, sr::Color::White );
}

const sr::Image& Game::getImage() const
{
    return image;
}

void Game::checkCollisions( Ball& ball )
{
    auto c = ball.getCircle();
    auto v = ball.getVelocity();

    const auto w = static_cast<float>( image.getWidth() );
    const auto h = static_cast<float>( image.getHeight() );

    if ( c.center.x - c.radius <= 0 )
    {
        c.center.x = c.radius;
        v.x *= -1;
    }
    else if ( c.center.x + c.radius > w )
    {
        c.center.x = w - c.radius;
        v.x *= -1;
    }

    if ( c.center.y - c.radius <= 0 )
    {
        c.center.y = c.radius;
        v.y *= -1;
    }
    else if ( c.center.y + c.radius >= h )
    {
        c.center.y = h - c.radius;
        v.y *= -1;
    }

    ball.setCircle( c );
    ball.setVelocity( v );
}
