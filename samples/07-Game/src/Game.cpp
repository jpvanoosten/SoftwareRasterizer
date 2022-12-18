#include <Color.hpp>
#include <Game.hpp>

#include <string>

using namespace sr;

Game::Game( uint32_t screenWidth, uint32_t screenHeight )
: image { screenWidth, screenHeight }
, arial20 { "assets/fonts/arial.ttf", 20 }
, arial24 { "assets/fonts/arial.ttf", 24 }
{
    background = Background { "assets/Pixel Adventure/Background/Blue.png", 1.0f, { 0.0f, 1.0f }, 0.3f };
}

void Game::Update()
{
    static double      totalTime = 0.0;
    static uint64_t    frames    = 0;
    static std::string fps       = "FPS: 0";

    timer.tick();
    ++frames;
    totalTime += timer.elapsedSeconds();
    if ( totalTime > 1.0 )
    {
        fps       = std::format( "FPS: {:.3f}", static_cast<double>( frames ) / totalTime );
        frames    = 0;
        totalTime = 0.0;
    }

    background.update( timer );

    image.clear( Color::Black );

    background.draw( image );

    // Draw an FPS counter in the corner of the screen.
    image.drawText( arial20, 6, 20, fps, Color::Black );
    image.drawText( arial20, 4, 18, fps, Color::White );
}

void Game::processEvent( const sr::Event& event )
{
    switch ( event.type )
    {
    case Event::None:
        break;
    case Event::Close:
        break;
    case Event::KeyPressed:
        onKeyPressed( event.key );
        break;
    case Event::KeyReleased:
        onKeyReleased( event.key );
        break;
    case Event::MouseMoved:
        onMouseMoved( event.mouseMove );
        break;
    case Event::MouseButtonPressed:
        break;
    case Event::MouseButtonReleased:
        break;
    case Event::MouseWheel:
        break;
    case Event::MouseHWheel:
        break;
    case Event::MouseEnter:
        break;
    case Event::MouseLeave:
        break;
    case Event::Resize:
        onResized( event.resize );
        break;
    case Event::EndResize:
        break;
    }
}

void Game::onKeyPressed( const sr::KeyEventArgs& args )
{
    switch ( args.code )
    {
    case KeyCode::Up:
        break;
    case KeyCode::Down:
        break;
    case KeyCode::Left:
        break;
    case KeyCode::Right:
        break;
    }
}

void Game::onKeyReleased( const sr::KeyEventArgs& args )
{
    switch ( args.code )
    {
    case KeyCode::Up:
        break;
    case KeyCode::Down:
        break;
    case KeyCode::Left:
        break;
    case KeyCode::Right:
        break;
    }
}

void Game::onMouseMoved( const sr::MouseMovedEventArgs& args )
{
    // Compute the mouse position relative to the game screen (which can be scaled if the window is resized).
    const glm::vec2 scale {
        static_cast<float>( image.getWidth() ) / static_cast<float>( gameRect.width ),
        static_cast<float>( image.getHeight() ) / static_cast<float>(gameRect.height)
    };
    const glm::vec2 offset { gameRect.topLeft() };
    
    mousePos = ( glm::vec2 { args.x, args.y } - offset ) * scale;
}

void Game::onResized( const sr::ResizeEventArgs& args )
{
    const float aspectRatio = static_cast<float>( image.getWidth() ) / static_cast<float>( image.getHeight() );
    const float scaleWidth  = static_cast<float>( args.width ) / static_cast<float>( image.getWidth() );
    const float scaleHeight = static_cast<float>( args.height ) / static_cast<float>( image.getHeight() );

    int width;
    int height;

    if ( scaleWidth < scaleHeight )
    {
        // Size according to the width.
        width  = args.width;
        height = static_cast<int>( static_cast<float>( width ) / aspectRatio );
    }
    else
    {
        // Size according to the height.
        height = args.height;
        width  = static_cast<int>( static_cast<float>( height ) * aspectRatio );
    }

    gameRect = {
        ( args.width - width ) / 2,
        ( args.height - height ) / 2,
        width, height
    };
}
