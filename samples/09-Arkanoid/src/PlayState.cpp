#include "Game.hpp"

#include <ParseRects.hpp>
#include <PlayState.hpp>

#include <Graphics/Input.hpp>

#include <glm/gtc/random.hpp>  // For generating random vectors.

using namespace Graphics;
using namespace Math;

PlayState::PlayState( Game& game )
: game { game }
, arcadeN { "assets/fonts/ARCADE_N.ttf", 7 }
, screenWidth { static_cast<int>( game.getImage().getWidth() ) }
, screenHeight { static_cast<int>( game.getImage().getHeight() ) }
{
    {
        // Load the vaus sprites.
        const auto spriteSheet = std::make_shared<SpriteSheet>( "assets/Arkanoid/vaus.png", ParseRects( "assets/Arkanoid/vaus.xml" ), BlendMode::AlphaBlend );
        // Set the paddle to the middle of the play area.
        vaus = Vaus { spriteSheet };
    }
    {
        // Load the field sprites.
        const auto spriteSheet = std::make_shared<SpriteSheet>( "assets/Arkanoid/fields.png", ParseRects( "assets/Arkanoid/fields.xml" ), BlendMode::Disable );
        field                  = Field { spriteSheet };
        field.setLives( numLives );
    }

    level = Level { game, levelId };
    field.setLevel( levelId );

    setState( State::Ready );
}

void PlayState::update( float deltaTime )
{
    field.setLives( numLives );
    level.update( deltaTime );

    switch ( state )
    {
    case State::Ready:
        doReady( deltaTime );
        break;
    case State::Appear:
        doAppear( deltaTime );
        break;
    case State::Start:
        doStart( deltaTime );
        break;
    case State::Playing:
        doPlaying( deltaTime );
        break;
    case State::Dead:
        doDead( deltaTime );
        break;
    }
}

void PlayState::drawText( Graphics::Image& image, std::string_view text, int x, int y )
{
    image.drawText( arcadeN, text, x + 1, y + 1, Color::Black );
    image.drawText( arcadeN, text, x, y, Color::White );
}

void PlayState::draw( Graphics::Image& image )
{
    // Clear the 16 pixels area above the game field.
    image.drawRectangle( RectUI { 0u, 0u, image.getWidth(), 16u }, Color::Black );

    // Draw the field.
    field.draw( image );

    switch ( state )
    {
    case State::Ready:
        drawText( image, std::format( "ROUND {:2}", levelId + 1 ), 81, 180 );
        if ( time > 1.0f )
            drawText( image, "READY", 93, 200 );
        break;
    case State::Start:
    case State::Playing:
        ball.draw( image );
        break;
    }

    level.draw( image );
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

PlayState::State PlayState::getState() const noexcept
{
    return state;
}

void PlayState::startState( State newState )
{
    switch ( newState )
    {
    case State::Ready:
        time = 0.0f;
        vaus.setState( Vaus::State::Wait );
        vaus.setPosition( glm::vec2 { static_cast<float>( screenWidth ) / 2.0f, static_cast<float>( screenHeight ) - 17.0f } );
        break;
    case State::Appear:
        level.animateBricks();
        vaus.setState( Vaus::State::Appear );
        break;
    case State::Start:
        time = 0.0f;
        break;
    case State::Playing:
        break;
    case State::Dead:
        vaus.setState( Vaus::State::Dead );
        break;
    }
}

void PlayState::endState( State oldState )
{
    switch ( oldState )
    {
    case State::Ready:
        break;
    case State::Appear:
        break;
    case State::Playing:
        break;
    case State::Dead:
        break;
    }
}

void PlayState::doReady( float deltaTime )
{
    time += deltaTime;

    if ( time > 2.0f )
    {
        setState( State::Appear );
    }
}

void PlayState::doAppear( float deltaTime )
{
    vaus.update( deltaTime );
    if ( vaus.getState() != Vaus::State::Appear )
    {
        setState( State::Start );
    }
}

void PlayState::doStart( float deltaTime )
{
    vaus.update( deltaTime );

    auto p    = vaus.getPosition();
    auto aabb = vaus.getAABB();

    // In the start state, the ball is attached to the paddle until the user presses the fire button.
    auto c   = ball.getCircle();
    c.center = { p.x, aabb.min.y - c.radius };
    ball.setCircle( c );

    time += deltaTime;

    if ( time > 3.0f || Input::getButtonDown( "Fire" ) )
    {
        // Initially, fire the ball up and to the right.
        glm::vec2 vel { 1.0f, 1.0f };
        if ( p.x < screenWidth / 2.0f )
        {
            // Or to the right when Vaus is on the left side of the screen.
            vel.x = -1.0f;
        }

        ball.setVelocity( normalize( vel ) * ballSpeed );
        setState( State::Playing );
    }
}

void PlayState::doPlaying( float deltaTime )
{
    vaus.update( deltaTime );
    ball.update( deltaTime );
    field.update( deltaTime );

#if _DEBUG
    // In debug mode, put vaus under the ball...
    auto x = ball.getPosition().x;
    auto y = vaus.getPosition().y;
//    vaus.setPosition( { x, y } );
#endif

    checkCollisions( ball );

    // No bricks left. Go to next level.
    if ( level.getNumBricks() == 0 )
    {
        // Go to the next level.
        ++levelId;
        level = Level { game, levelId };
        field.setLevel( levelId );
        setState( State::Ready );
    }
}

void PlayState::checkCollisions( Ball& ball )
{
    auto c = ball.getCircle();
    auto v = ball.getVelocity();

    // Level bounds.
    constexpr float top    = 24.0f;
    constexpr float bottom = 256.0f;
    constexpr float left   = 8.0f;
    constexpr float right  = 216.0f;

    // Check collision with the sides of the level.
    if ( c.center.x - c.radius <= left )
    {
        c.center.x = left + c.radius;
        v.x *= -1;
    }
    else if ( c.center.x + c.radius > right )
    {
        c.center.x = right - c.radius;
        v.x *= -1;
    }

    if ( c.center.y - c.radius <= top )
    {
        c.center.y = top + c.radius;
        v.y *= -1;
    }
    else if ( c.center.y + c.radius >= bottom )
    {
#if _DEBUG
        c.center.y = bottom - c.radius;
        v.y *= -1;
#else
        setState( State::Dead );
#endif
    }

    if ( const auto hit = vaus.collidesWith( ball ) )
    {
        // Slightly perturb the hit normal to prevent the ball from getting stuck on gold bricks.
        glm::vec2 n = glm::normalize( hit->normal + glm::diskRand( 0.2f ) );
        c.center    = hit->point + n * c.radius;
        // Reflect the velocity of the ball about the hit normal.
        v = glm::reflect( v, hit->normal );
    }

    if ( const auto hit = level.checkCollision( ball ) )
    {
        // Slightly perturb the hit normal to prevent the ball from getting stuck on gold bricks.
        glm::vec2 n = glm::normalize( hit->normal + glm::diskRand( 0.2f ) );
        c.center    = hit->point + n * c.radius;
        // Reflect the velocity of the ball about the hit normal.
        v = glm::reflect( v, hit->normal );
    }

    ball.setCircle( c );
    ball.setVelocity( v );
}

void PlayState::doDead( float deltaTime )
{
    vaus.update( deltaTime );
    if ( vaus.getState() == Vaus::State::Wait )
    {
        if ( numLives > 0 )
        {
            --numLives;
            setState( State::Ready );
        }
        else
        {
            setState( State::GameOver );
        }
    }
}
