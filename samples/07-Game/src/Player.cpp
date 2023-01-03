#include "Player.hpp"

#include "Font.hpp"

#include <Player.hpp>

#include <Input.hpp>
#include <SpriteAnim.hpp>
#include <iostream>

#include <numbers>

// A map to convert the player state to a string (for debugging).
std::map<Player::State, std::string> stateToString = {
    { Player::State::Idle, "Idle" },
    { Player::State::Run, "Run" },
    { Player::State::Jump, "Jump" },
    { Player::State::Hit, "Hit" },
    { Player::State::DoubleJump, "Double Jump" },
    { Player::State::Falling, "Falling" },
    { Player::State::WallJump, "Wall Jump" }
};

using namespace sr;

Character createCharacter( const std::filesystem::path& basePath )
{
    SpriteAnim doubleJump { basePath / "Double Jump (32x32).png", 20, 32, 32 };
    SpriteAnim fall { basePath / "Fall (32x32).png", 20, 32, 32 };
    SpriteAnim hit { basePath / "Hit (32x32).png", 20, 32, 32 };
    SpriteAnim idle { basePath / "Idle (32x32).png", 20, 32, 32 };
    SpriteAnim jump { basePath / "Jump (32x32).png", 20, 32, 32 };
    SpriteAnim run { basePath / "Run (32x32).png", 20, 32, 32 };
    SpriteAnim wallJump { basePath / "Wall Jump (32x32).png", 20, 32, 32 };

    Character character;

    character.addAnimation( "Double Jump", std::move( doubleJump ) );
    character.addAnimation( "Fall", std::move( fall ) );
    character.addAnimation( "Hit", std::move( hit ) );
    character.addAnimation( "Idle", std::move( idle ) );
    character.addAnimation( "Jump", std::move( jump ) );
    character.addAnimation( "Run", std::move( run ) );
    character.addAnimation( "Wall Jump", std::move( wallJump ) );

    return character;
}

Player::Player( const Math::Transform2D& transform )
: transform { transform }
{
    characters.emplace_back( createCharacter( "assets/Pixel Adventure/Main Characters/Mask Dude" ) );
    characters.emplace_back( createCharacter( "assets/Pixel Adventure/Main Characters/Ninja Frog" ) );
    characters.emplace_back( createCharacter( "assets/Pixel Adventure/Main Characters/Pink Man" ) );
    characters.emplace_back( createCharacter( "assets/Pixel Adventure/Main Characters/Virtual Guy" ) );

    currentCharacter = characters.begin();

    currentCharacter->setAnimation( "Idle" );

    // Setup an acceleration curve for running.
    accelCurve.setFunc( []( float x ) {
        if ( x > 0.0f && x < 0.25f )
            return x * 4;

        if ( x <= 0.0f )
            return 0.0f;

        return 1.0f;
    } );
}

void Player::reset()
{
    if ( ++currentCharacter == characters.end() )
    {
        currentCharacter = characters.begin();
    }

    currentCharacter->setAnimation( "Idle" );
    setState( State::Idle );
}

void Player::update( float deltaTime ) noexcept
{
    switch ( state )
    {
    case State::Idle:
        doIdle( deltaTime );
        break;
    case State::Run:
        doRun( deltaTime );
        break;
    case State::Jump:
        doJump( deltaTime );
        break;
    case State::Hit:
        doHit( deltaTime );
        break;
    case State::DoubleJump:
        doDoubleJump( deltaTime );
        break;
    case State::Falling:
        doFalling( deltaTime );
        break;
    case State::WallJump:
        doWallJump( deltaTime );
        break;
    }

    transform.translate( glm::vec2 { velocity.x, -velocity.y } * deltaTime );

    if ( currentCharacter != characters.end() )
    {
        currentCharacter->update( deltaTime );
    }
}

void Player::setTransform( const Math::Transform2D& _transform )
{
    transform = _transform;
}

const Math::Transform2D& Player::getTransform() const noexcept
{
    return transform;
}

void Player::draw( sr::Image& image ) const noexcept
{
    if ( currentCharacter != characters.end() )
    {
        currentCharacter->draw( image, transform );
    }

#if _DEBUG
    // Draw the current state of the player.
    auto pos = transform.getPosition() - glm::vec2 { 12, 50 };
    image.drawText( Font::Default, static_cast<int>( pos.x ), static_cast<int>( pos.y ), stateToString[state], Color::White );
#endif
}

void Player::setState( State newState )
{
    if ( state != newState )
    {
        endState( state );
        startState( newState );
        state = newState;
    }
}

void Player::startState( State newState )
{
    switch ( newState )
    {
    case State::Idle:
        currentCharacter->setAnimation( "Idle" );
        accelCurve    = 0.0f;
        canDoubleJump = true;
        break;
    case State::Run:
        currentCharacter->setAnimation( "Run" );
        canDoubleJump = true;
        break;
    case State::Jump:
        currentCharacter->setAnimation( "Jump" );
        velocity.y    = jumpSpeed;
        canDoubleJump = true;
        break;
    case State::Hit:
        currentCharacter->setAnimation( "Hit" );
        velocity.y = jumpSpeed / 2.0f;
        break;
    case State::DoubleJump:
        currentCharacter->setAnimation( "Double Jump" );
        velocity.y    = jumpSpeed;
        canDoubleJump = false;
        break;
    case State::Falling:
        currentCharacter->setAnimation( "Fall" );
        break;
    case State::WallJump:
        currentCharacter->setAnimation( "Wall Jump" );
        break;
    case State::Dead:
        std::cout << "Player died..." << std::endl;
        break;
    }
}

void Player::endState( State oldState )
{
}

float Player::doHorizontalMovement( float deltaTime )
{
    accelCurve += deltaTime;

    const float horizontal = Input::getAxis( "Horizontal" ) * accelCurve() * playerSpeed;

    if ( horizontal < 0.0f )
    {
        transform.setScale( { -1, 1 } );
    }
    else if ( horizontal > 0.0f )
    {
        transform.setScale( { 1, 1 } );
    }

    return horizontal;
}

void Player::doIdle( float deltaTime )
{
    if ( Input::getAxis( "Horizontal" ) != 0.0f )
    {
        setState( State::Run );
    }

    if ( Input::getButtonDown( "Jump" ) )
    {
        setState( State::Jump );
    }
}

void Player::doRun( float deltaTime )
{
    const float horizontal = doHorizontalMovement( deltaTime );
    velocity.x             = horizontal;

    if ( Input::getButtonDown( "Jump" ) )
    {
        setState( State::Jump );
    }
    else if ( horizontal == 0.0f )
    {
        setState( State::Idle );
    }
}

void Player::doJump( float deltaTime )
{
    const float horizontal = doHorizontalMovement( deltaTime );
    velocity.x             = horizontal;

    // Apply gravity
    velocity.y -= gravity * deltaTime;

    if ( canDoubleJump && Input::getButtonDown( "Jump" ) )
    {
        setState( State::DoubleJump );
    }
    else if ( velocity.y < 0.0f )
    {
        setState( State::Falling );
    }
}

void Player::doHit( float deltaTime )
{
    // Apply gravity:
    velocity.y -= gravity * deltaTime;

    // Rotate the player
    transform.rotate( std::numbers::pi_v<float> * deltaTime );

    // If player is off the bottom of the screen...
    if ( transform.getPosition().y > 1000.0f )
    {
        // TODO: Player dead... Reset level.
        setState( State::Dead );
    }
}

void Player::doDoubleJump( float deltaTime )
{
    const float horizontal = doHorizontalMovement( deltaTime );
    velocity.x             = horizontal;

    // Apply gravity
    velocity.y -= gravity * deltaTime;

    if ( velocity.y < 0.0f )
    {
        setState( State::Falling );
    }
}

void Player::doFalling( float deltaTime )
{
    const float horizontal = doHorizontalMovement( deltaTime );
    velocity.x             = horizontal;

    // Apply gravity
    velocity.y -= gravity * deltaTime;

    if ( canDoubleJump && Input::getButtonDown( "Jump" ) )
    {
        setState( State::DoubleJump );
    }

    auto pos = transform.getPosition();
    if ( pos.y > 256 )
    {
        pos.y      = 256;
        velocity.y = 0.0f;
        transform.setPosition( pos );
        setState( velocity.x == 0.0 ? State::Idle : State::Run );
    }
}

void Player::doWallJump( float deltaTime )
{
    // TODO:
}
