#include <Game.hpp>
#include <Color.hpp>

#include <string>

using namespace sr;

Game::Game( uint32_t screenWidth, uint32_t screenHeight )
: image { screenWidth, screenHeight }
, arial24 { "assets/fonts/arial.ttf", 24 }
, arial56 { "assets/fonts/arial.ttf", 56 }
{}

void Game::Update()
{
    static double      totalTime = 0.0;
    static uint64_t    frames    = 0;
    static std::string fps       = "FPS: 0";

    timer.tick();
    ++frames;
    totalTime += timer.elapsedSeconds();
    if (totalTime > 1.0)
    {
        fps        = std::format( "FPS: {:.3f}", static_cast<double>( frames ) / totalTime );
        frames    = 0;
        totalTime  = 0.0;
    }

    image.clear( Color::Black );

    image.drawText( arial24, 10, 24, fps, Color::White );

    // Draw some text centered on the screen.
    {
        const std::string text = "The quick brown fox jumps\nover the lazy dog.";
        const auto        size = arial56.getSize( text );
        int               x    = ( static_cast<int>( image.getWidth() ) - size.x ) / 2;
        int               y    = ( static_cast<int>( image.getHeight() ) - size.y ) / 2;
        image.drawText( arial56, x, y, text, Color::White );
    }
}
