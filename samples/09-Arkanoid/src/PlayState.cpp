#include "Game.hpp"
#include "PowerUp.hpp"

#include <ParseRects.hpp>
#include <PlayState.hpp>

#include <Graphics/Input.hpp>

#include <fmt/core.h>
#include <glm/gtc/random.hpp>  // For generating random vectors.

using namespace Graphics;
using namespace Math;

// Get the spritesheet for the powerups.
std::shared_ptr<SpriteSheet> GetPowerUpSprites()
{
    static std::shared_ptr<SpriteSheet> sprites = std::make_shared<SpriteSheet>( "assets/Arkanoid/powerups.png", ParseRects( "assets/Arkanoid/powerups.xml" ), BlendMode::AlphaBlend );
    return sprites;
}

const PowerUp& GetPowerup( PowerUp::Type type )
{
    static auto    sprites    = GetPowerUpSprites();
    static PowerUp powerUps[] = {
        { nullptr, {}, PowerUp::None },
        { sprites, { { 0, 1, 2, 3, 4, 5, 6, 7 } }, PowerUp::Slow },
        { sprites, { { 8, 9, 10, 11, 12, 13, 14, 15 } }, PowerUp::Catch },
        { sprites, { { 16, 17, 18, 19, 20, 21, 22, 23 } }, PowerUp::Laser },
        { sprites, { { 24, 25, 26, 27, 28, 29, 30, 31 } }, PowerUp::Enlarge },
        { sprites, { { 32, 33, 34, 35, 36, 37, 38, 39 } }, PowerUp::Disruption },
        { sprites, { { 40, 41, 42, 43, 44, 45, 46, 47 } }, PowerUp::Break },
        { sprites, { { 48, 49, 50, 51, 52, 53, 54, 55 } }, PowerUp::Player }
    };

    return powerUps[type];
}

PlayState::PlayState( Game& game )
: game { game }
, screenWidth { static_cast<int>( game.getImage().getWidth() ) }
, screenHeight { static_cast<int>( game.getImage().getHeight() ) }
, rng { std::random_device {}() }  // Seed the random number generator with a random device.
{
    // Input that controls the horizontal movement of the paddle.
    Input::mapAxis( "Horizontal", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        float leftX = 0.0f;

        for ( auto& gamePadState: gamePadStates )
        {
            const auto state = gamePadState.getLastState();

            leftX += state.thumbSticks.leftX;
        }

        const auto keyState = keyboardState.getLastState();

        const float a     = keyState.A ? 1.0f : 0.0f;
        const float d     = keyState.D ? 1.0f : 0.0f;
        const float left  = keyState.Left ? 1.0f : 0.0f;
        const float right = keyState.Right ? 1.0f : 0.0f;

        return std::clamp( leftX - a + d - left + right, -1.0f, 1.0f );
    } );

    // Input that controls shooting.
    Input::mapButtonDown( "Fire", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool a = false;

        for ( auto& gamePadState: gamePadStates )
        {
            a = a || gamePadState.a == ButtonState::Pressed;
        }

        const bool space = keyboardState.isKeyPressed( KeyCode::Space );
        const bool up    = keyboardState.isKeyPressed( KeyCode::Up );
        const bool w     = keyboardState.isKeyPressed( Graphics::KeyCode::W );

        return a || space || up || w;
    } );

    {
        // Load the vaus sprites.
        const auto spriteSheet = std::make_shared<SpriteSheet>( "assets/Arkanoid/vaus.png", ParseRects( "assets/Arkanoid/vaus.xml" ), BlendMode::AlphaBlend );
        // Set the paddle to the middle of the play area.
        vaus = Vaus { spriteSheet };
        // Initialize the bullets.
        bullets[0] = bullets[1] = Bullet { spriteSheet, { { 33, 34, 35 } } };
    }
    {
        // Load the field sprites.
        const auto spriteSheet = std::make_shared<SpriteSheet>( "assets/Arkanoid/fields.png", ParseRects( "assets/Arkanoid/fields.xml" ), BlendMode::Disable );
        field                  = Field { spriteSheet };
        field.setLives( numLives[currentPlayer] );
    }

    level = Level { game, levelId };
    field.setLevel( levelId );
    balls.emplace_back();

    setState( State::Ready );
}

void PlayState::update( float deltaTime )
{
    field.setLives( numLives[currentPlayer] );
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
    const auto& font = game.getFont();

    image.drawText( font, text, x + 1, y + 1, Color::Black );
    image.drawText( font, text, x, y, Color::White );
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
        drawText( image, fmt::format( "ROUND {:2}", levelId + 1 ), 81, 180 );
        if ( time > 1.0f )
            drawText( image, "READY", 93, 200 );
        break;
    case State::Start:
    case State::Playing:
        for ( auto& ball: balls )
            ball.draw( image );
        break;
    }

    level.draw( image );
    vaus.draw( image );

    // draw bullets
    for ( auto& bullet: bullets )
    {
        bullet.draw( image );
    }

    // Draw power-ups
    for ( auto& powerUp: powerUps )
    {
        powerUp.draw( image );
    }
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
        for ( auto& bullet: bullets )
        {
            bullet.setState( Bullet::State::None );
        }
        powerUps.clear();
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
    auto c   = balls[0].getCircle();
    c.center = { p.x, aabb.min.y - c.radius };
    balls[0].setCircle( c );

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

        balls[0].setVelocity( normalize( vel ) * ballSpeed );

        setState( State::Playing );
    }
}

void PlayState::doPlaying( float deltaTime )
{
#if _DEBUG
    // In debug mode, put vaus under the ball...
    auto x = balls[0].getPosition().x;
    auto y = vaus.getPosition().y;
    vaus.setPosition( { x, y } );
#endif

    vaus.update( deltaTime );

    for ( auto& ball: balls )
        ball.update( deltaTime );

    for ( auto& powerUp: powerUps )
        powerUp.update( deltaTime );

    field.update( deltaTime );

    if ( vaus.getState() == Vaus::State::Laser )
    {
        bulletTimeout -= deltaTime;
        if ( Input::getButtonDown( "Fire" ) && bulletTimeout < 0.0f )
        {
            bulletTimeout = 0.1f;  // Don't fire a bullet again for x seconds.
            for ( auto& bullet: bullets )
            {
                if ( bullet.getState() == Bullet::State::None )
                {
                    bullet.fire( vaus.getPosition() );
                    break;
                }
            }
        }
    }

    for ( auto& bullet: bullets )
    {
        bullet.update( deltaTime );
        checkCollisions( bullet );
    }

    for ( auto& ball: balls )
        checkCollisions( ball );

    // Handle power-ups.
    for ( auto iter = powerUps.begin(); iter != powerUps.end(); )
    {
        auto& powerUp = *iter;
        if ( checkCollisions( powerUp ) )
        {
            iter = powerUps.erase( iter );
        }
        else
        {
            ++iter;
        }
    }

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
        // Use the hit normal to determine the direction of the ball.
        v = hit->normal * ballSpeed;
    }

    if ( const auto hit = level.checkCollision( ball, *this ) )
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

void PlayState::checkCollisions( Bullet& bullet )
{
    // Only check collision with fired bullets.
    if ( bullet.getState() != Bullet::State::Fired )
        return;

    // Level bounds.
    constexpr float top = 24.0f;

    Math::AABB aabb = bullet.getAABB();

    // Check for collision with the bricks.
    if ( level.checkCollision( bullet, *this ) )
    {
        bullet.hit();
    }

    // Check for collision with the top of the screen.
    if ( aabb.min.y <= top )
    {
        bullet.hit();
    }
}

bool PlayState::checkCollisions( const PowerUp& powerUp )
{
    // Destroy power-ups that reach the bottom of the screen.
    if ( powerUp.getPosition().y > 250.0f )
        return true;

    if ( powerUp.checkCollision( vaus.getAABB() ) )
    {
        switch ( powerUp.getType() )
        {
        case PowerUp::Slow:
            break;
        case PowerUp::Catch:
            break;
        case PowerUp::Laser:
            if ( vaus.getState() != Vaus::State::Laser )
                vaus.setState( Vaus::State::ToLaser );
            break;
        case PowerUp::Enlarge:
            if ( vaus.getState() != Vaus::State::Enlarge )
                vaus.setState( Vaus::State::Enlarge );
            break;
        case PowerUp::Disruption:
            break;
        case PowerUp::Break:
            break;
        case PowerUp::Player:
            // Max 13 lives...
            numLives[currentPlayer] = std::min( 13, numLives[currentPlayer] + 1 );
            break;
        }

        return true;
    }

    return false;
}

void PlayState::spawnPowerUp( const glm::vec2& pos )
{
    if ( powerUpChance( rng ) )
    {
        const auto type = static_cast<PowerUp::Type>( powerUpTypeDist( rng ) );
        assert( type != PowerUp::None );
        auto powerUp = GetPowerup( type );
        powerUp.setPosition( pos );

        powerUps.push_back( powerUp );
    }
}

void PlayState::doDead( float deltaTime )
{
    vaus.update( deltaTime );
    if ( vaus.getState() == Vaus::State::Wait )
    {
        if ( numLives[currentPlayer] > 0 )
        {
            --numLives[currentPlayer];
            setState( State::Ready );
        }
        else
        {
            setState( State::GameOver );
        }
    }
}
