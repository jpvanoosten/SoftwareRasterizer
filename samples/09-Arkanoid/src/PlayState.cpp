#include "Game.hpp"

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

PlayState::PlayState( Game& game )
: game { game }
, screenWidth { static_cast<int>( game.getImage().getWidth() ) }
, screenHeight { static_cast<int>( game.getImage().getHeight() ) }
{
    {
        // Load the vaus sprites.
        const auto spriteSheet = std::make_shared<SpriteSheet>( "assets/Arkanoid/vaus.png", parseSpriteRects( "assets/Arkanoid/vaus.xml" ), BlendMode::AlphaBlend );
        // Set the paddle to the middle of the play area.
        vaus = Vaus { spriteSheet, glm::vec2 { static_cast<float>( screenWidth ) / 2.0f, static_cast<float>( screenHeight ) - 17.0f } };
    }
    {
        // Load the field sprites.
        const auto spriteSheet = std::make_shared<SpriteSheet>( "assets/Arkanoid/fields.png", parseSpriteRects( "assets/Arkanoid/fields.xml" ), BlendMode::Disable );
        field                  = Field { spriteSheet };
    }
}

void PlayState::update( float deltaTime )
{
    switch ( state )
    {
    case State::Start:
        doStart( deltaTime );
        break;
    case State::Playing:
        doPlaying( deltaTime );
        break;
    case State::Dead:
        break;
    }
}

void PlayState::draw( Graphics::Image& image )
{
    // Clear the area above the game field.
    image.drawRectangle( RectUI { 0u, 0u, image.getWidth(), 16u }, Color::Black );

    field.draw( image );
    ball.draw( image );
    vaus.draw( image );
}

void PlayState::setState( State newState )
{
    if ( state != newState )
    {
        endState( state );
        state = newState;
        startState( state );
    }
}

void PlayState::startState( State newState )
{
    switch ( newState )
    {
    case State::Start:
        break;
    case State::Playing:
        break;
    case State::Dead:
        break;
    }
}

void PlayState::endState( State oldState )
{
    switch ( oldState )
    {
    case State::Start:
        break;
    case State::Playing:
        break;
    case State::Dead:
        break;
    }
}

void PlayState::doStart( float deltaTime )
{
    updatePaddle( deltaTime );
    auto p    = vaus.getPosition();
    auto aabb = vaus.getAABB();

    // In the start state, the ball is attached to the paddle until the user presses the fire button.
    auto c   = ball.getCircle();
    c.center = { p.x, aabb.min.y - c.radius };
    ball.setCircle( c );

    if ( Input::getButtonDown( "Fire" ) )
    {
        ball.setVelocity( glm::vec2 { 0.0f, 1.0f } * ballSpeed );
        setState( State::Playing );
    }
}

void PlayState::doPlaying( float deltaTime )
{
    updatePaddle( deltaTime );

    ball.update( deltaTime );
    field.update( deltaTime );

    checkCollisions( ball );
}

void PlayState::updatePaddle( float deltaTime )
{
    const auto w = static_cast<float>( screenWidth );

    auto pos = vaus.getPosition();
    pos.x += Input::getAxis( "Horizontal" ) * paddleSpeed * deltaTime;

    if ( pos.x < 0.0f )
        pos.x = 0.0f;
    else if ( pos.x >= w )
        pos.x = w;

    vaus.setPosition( pos );
    vaus.update( deltaTime );
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

    if ( const auto hit = vaus.collidesWith( ball ) )
    {
        c.center = hit->point + hit->normal * c.radius;
        // Reflect the velocity of the ball about the hit normal.
        v = glm::reflect( v, hit->normal );
    }

    ball.setCircle( c );
    ball.setVelocity( v );
}
