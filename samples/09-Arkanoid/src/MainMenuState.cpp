#include "Game.hpp"

#include <MainMenuState.hpp>
#include <iostream>

#include <Graphics/ResourceManager.hpp>

using namespace Graphics;

MainMenuState::MainMenuState( Game& game )
: game { game }
, screenWidth( static_cast<int>( game.getImage().getWidth() ) )
, screenHeight( static_cast<int>( game.getImage().getHeight() ) )
, arcadeN { "assets/fonts/ARCADE_N.TTF", 8, 32, 138 }
, coprgtb { "assets/fonts/COPRGTB.TTF", 24 }
{
    auto shipImage = ResourceManager::loadImage( "assets/Arkanoid/ship.png" );
    arkanoidSprite = Sprite( shipImage, { 0, 0, 193, 42 }, BlendMode::AlphaBlend );
}

void MainMenuState::update( float deltaTime )
{
    // TODO: Use input controller to select/press buttons.
}

void MainMenuState::draw( Graphics::Image& image )
{
    image.clear( Color::Black );

    image.drawSprite( arkanoidSprite, 16, 63 );

    int coins = game.getCoins();
    if (coins > 0)
    {
        image.drawText( arcadeN, 97, 140, "PUSH", Color::White );
        if ( coins < 2 )
        {
            image.drawText( arcadeN, 33, 168, "ONLY 1 PLAYER BUTTON", Color::White );
        }
        else
        {
            image.drawText( arcadeN, 34, 168, "1 OR 2 PLAYER BUTTON", Color::White );
        }
    }

    image.drawText( coprgtb, 73, 225, "TAITO", Color::Red );
    image.drawText( arcadeN, 16, 243, L"© 1986 TAITO CORP JAPAN", Color::White );
    image.drawText( arcadeN, 33, 261, "ALL RIGHTS RESERVED", Color::White );
    image.drawText( arcadeN, 145, 298, std::format( "CREDIT{:3d}", coins ), Color::White );
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
