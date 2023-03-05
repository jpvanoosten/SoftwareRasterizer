#include <MainMenuState.hpp>
#include <iostream>

#include <Graphics/ResourceManager.hpp>

using namespace Graphics;

MainMenuState::MainMenuState( int screenWidth, int screenHeight )
: screenWidth { screenWidth }
, screenHeight { screenHeight }
, buttonFont { ResourceManager::loadFont( "assets/fonts/kenvector_future.ttf", buttonHeight ) }
, startButton { "play", buttonFont, Color::White }
, quitButton { "quit", buttonFont, Color::White }
{
    startButton.setRect( { static_cast<float>( screenWidth - buttonWidth ) / 2.0f, static_cast<float>( screenHeight - buttonHeight ) / 2.0f, static_cast<float>( buttonWidth ), static_cast<float>( buttonHeight ) } );
    quitButton.setRect( { static_cast<float>( screenWidth - buttonWidth ) / 2.0f, static_cast<float>( screenHeight - buttonHeight ) / 2.0f + buttonHeight * 2.0f, static_cast<float>( buttonWidth ), static_cast<float>( buttonHeight ) } );

    startButton.setCallback( [this] {
        // TODO: What to do when the start button is pressed?
        std::cout << "Start pressed!" << std::endl;
    } );

    quitButton.setCallback( [this] {
        // TODO: What to do when the quit button is pressed?
        std::cout << "Quit pressed!" << std::endl;
    } );
}

void MainMenuState::update( float deltaTime )
{
    // TODO: Use input controller to select/press buttons.
}

void MainMenuState::draw( Graphics::Image& image )
{
    image.clear( Color::Black );

    startButton.draw( image );
    quitButton.draw( image );
}

void MainMenuState::processEvent( const Graphics::Event& _event )
{

    // Copy the event so we can modify it.
    Event event = _event;

    switch ( event.type )
    {
    case Event::None:
        break;
    case Event::Close:
        break;
    case Event::KeyPressed:
        break;
    case Event::KeyReleased:
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

    startButton.processEvents( event );
    quitButton.processEvents( event );
}

void MainMenuState::onMouseMoved( Graphics::MouseMovedEventArgs& args )
{
    // Compute the mouse position relative to the menu screen (which can be scaled if the window is resized).
    const glm::vec2 scale {
        static_cast<float>( screenWidth ) / static_cast<float>( gameRect.width ),
        static_cast<float>( screenHeight ) / static_cast<float>( gameRect.height )
    };
    const glm::vec2 offset { gameRect.topLeft() };

    args.x = std::lround( ( static_cast<float>( args.x ) - offset.x ) * scale.x );
    args.y = std::lround( ( static_cast<float>( args.y ) - offset.y ) * scale.y );

    mousePos = { args.x, args.y };
}

void MainMenuState::onResized( const Graphics::ResizeEventArgs& args )
{
    const float aspectRatio = static_cast<float>( screenWidth ) / static_cast<float>( screenHeight );
    const float scaleWidth  = static_cast<float>( args.width ) / static_cast<float>( screenWidth );
    const float scaleHeight = static_cast<float>( args.height ) / static_cast<float>( screenHeight );

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
