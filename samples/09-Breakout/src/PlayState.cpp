#include <PlayState.hpp>

#include <Graphics/Input.hpp>

#include "tinyxml2.h"

using namespace Graphics;
using namespace tinyxml2;
using namespace Math;

// Parse the XML file and return a list of rects containing the sprites in the texture atlas.
std::vector<RectI> parseSpriteRects( const std::filesystem::path& xmlFile )
{
    XMLDocument doc;
    doc.LoadFile( xmlFile.string().c_str() );

    XMLElement* root = doc.RootElement();

    std::vector<RectI> rects;

    for ( XMLElement* subTexture = root->FirstChildElement( "SubTexture" ); subTexture != nullptr; subTexture = subTexture->NextSiblingElement( "SubTexture" ) )
    {
        int x      = subTexture->IntAttribute( "x" );
        int y      = subTexture->IntAttribute( "y" );
        int width  = subTexture->IntAttribute( "width" );
        int height = subTexture->IntAttribute( "height" );
        rects.emplace_back( x, y, width, height );
    }

    return rects;
}

PlayState::PlayState( int screenWidth, int screenHeight )
: backgroundImage { "assets/textures/space_background.jpg" }
, screenWidth { screenWidth }
, screenHeight { screenHeight }
{
    // Load the game sprites.
    spriteSheet = std::make_shared<SpriteSheet>( "assets/Breakout/Sprite Sheet/Breakout_Tile_Free.png", parseSpriteRects( "assets/Breakout/Sprite Sheet/Breakout_Tile_Free.xml" ), Graphics::BlendMode::AlphaBlend );

    paddle = Paddle { spriteSheet, { static_cast<float>( screenWidth ) / 2.0f, static_cast<float>( screenHeight ) - 100.0f } };

    ball.setPosition( { static_cast<float>( screenWidth ) / 2.0f, static_cast<float>( screenHeight ) / 2.0f } );
    ball.setVelocity( glm::vec2 { 1, -1 } * 300.0f );
}

void PlayState::update( float deltaTime )
{
    const auto w = static_cast<float>( screenWidth );

    // Update the paddle.
    {
        auto pos = paddle.getPosition();
        pos.x += Input::getAxis( "Horizontal" ) * paddleSpeed;

        if ( pos.x < 0.0f )
            pos.x = 0.0f;
        else if ( pos.x >= w )
            pos.x = w;

        paddle.setPosition( pos );

        paddle.update( deltaTime );
    }

    ball.update( deltaTime );
    checkCollisions( ball );
}

void PlayState::draw( Graphics::Image& image )
{
    image.copy( backgroundImage, 0, 0 );

    paddle.draw( image );
    ball.draw( image );
}

void PlayState::processEvent( const Graphics::Event& event )
{
    
}

void PlayState::checkCollisions( Ball& ball )
{
    auto c = ball.getCircle();
    auto v = ball.getVelocity();

    const auto w = static_cast<float>( screenWidth );
    const auto h = static_cast<float>( screenHeight );

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

    if ( paddle.collidesWith( c ) )
    {
        v.y *= -1;
    }

    ball.setCircle( c );
    ball.setVelocity( v );
}
