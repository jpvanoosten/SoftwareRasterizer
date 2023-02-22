#include <Player.hpp>

#include <Font.hpp>
#include <Input.hpp>
#include <ResourceManager.hpp>
#include <SpriteAnim.hpp>

#include <iostream>
#include <numbers>

// How fast the player accelerates.
const float Player::accel = 5e3f;
// The maximum speed of the player.
const float Player::maxSpeed = 150.0f;
// Maximum jump height in pixels.
const float Player::jumpHeight = 60.0f;
// Jump time is the time (in seconds) to the apex of the jump.
const float Player::jumpTime = 0.25f;
// Gravity and jump speed are computed to achieve the desired jump parabola.
// Source: https://2dengine.com/?p=platformers#Jumping
// Source: https://jobtalle.com/2d_platformer_physics.html
const float Player::gravity   = 2.0f * jumpHeight / ( jumpTime * jumpTime );
const float Player::jumpSpeed = std::sqrt( 2.0f * jumpHeight * gravity );
// How fast the player jumps off the wall.
const float Player::wallJumpSpeed = 500.0f;
// How long after falling can the player still jump?
const float Player::coyoteTime = 0.1f;
// How long before landing the user can trigger a jump.
const float Player::jumpBuffer = 0.1f;

// A map to convert the player state to a string (for debugging).
std::map<Player::State, std::string> stateToString = {
    { Player::State::Idle, "Idle" },
    { Player::State::Run, "Run" },
    { Player::State::Jump, "Jump" },
    { Player::State::Hit, "Hit" },
    { Player::State::DoubleJump, "Double Jump" },
    { Player::State::Falling, "Falling" },
    { Player::State::LeftWallJump, "Left Wall Jump" },
    { Player::State::RightWallJump, "Right Wall Jump" }
};

using namespace sr;

std::shared_ptr<Character> createCharacter( const std::filesystem::path& basePath )
{
    auto character = std::make_shared<Character>();

    // Load the sprite sheets for the character.
    const auto doubleJump = ResourceManager::loadSpriteSheet( basePath / "Double Jump (32x32).png", 32, 32, 0, 0, BlendMode::AlphaBlend );
    const auto fall       = ResourceManager::loadSpriteSheet( basePath / "Fall (32x32).png", 32, 32, 0, 0, BlendMode::AlphaBlend );
    const auto hit        = ResourceManager::loadSpriteSheet( basePath / "Hit (32x32).png", 32, 32, 0, 0, BlendMode::AlphaBlend );
    const auto idle       = ResourceManager::loadSpriteSheet( basePath / "Idle (32x32).png", 32, 32, 0, 0, BlendMode::AlphaBlend );
    const auto jump       = ResourceManager::loadSpriteSheet( basePath / "Jump (32x32).png", 32, 32, 0, 0, BlendMode::AlphaBlend );
    const auto run        = ResourceManager::loadSpriteSheet( basePath / "Run (32x32).png", 32, 32, 0, 0, BlendMode::AlphaBlend );
    const auto wallJump   = ResourceManager::loadSpriteSheet( basePath / "Wall Jump (32x32).png", 32, 32, 0, 0, BlendMode::AlphaBlend );

    // Add the sprite animations for the character.
    character->addAnimation( "Double Jump", SpriteAnim { doubleJump, 20 } );
    character->addAnimation( "Fall", SpriteAnim { fall, 20 } );
    character->addAnimation( "Hit", SpriteAnim { hit, 20 } );
    character->addAnimation( "Idle", SpriteAnim { idle, 20 } );
    character->addAnimation( "Jump", SpriteAnim { jump, 20 } );
    character->addAnimation( "Run", SpriteAnim { run, 20 } );
    character->addAnimation( "Wall Jump", SpriteAnim { wallJump, 20 } );

    return character;
}

Player::Player( const Math::Transform2D& _transform )
: transform { _transform }
, aabb { { 7, 5, 0 }, { 25, 32, 0 } }  // Player is 32x32 pixels, but the AABB should be slightly smaller.
, topAABB { { 7, 3, 0 }, { 25, 5, 0 } }
, bottomAABB { { 7, 30, 0 }, { 25, 32, 0 } }
, leftAABB { { 5, 5, 0 }, { 7, 29, 0 } }
, rightAABB { { 25, 5, 0 }, { 27, 29, 0 } }
{
    // Player sprite is 32x32 pixels.
    // Place the anchor point in the bottom center of the sprite.
    transform.setAnchor( { 16, 32 } );

    characters.emplace_back( createCharacter( "assets/Pixel Adventure/Main Characters/Mask Dude" ) );
    characters.emplace_back( createCharacter( "assets/Pixel Adventure/Main Characters/Ninja Frog" ) );
    characters.emplace_back( createCharacter( "assets/Pixel Adventure/Main Characters/Pink Man" ) );
    characters.emplace_back( createCharacter( "assets/Pixel Adventure/Main Characters/Virtual Guy" ) );

    currentCharacter = characters[0];
    currentCharacter->setAnimation( "Idle" );

    jumpSound.loadSound( "assets/sounds/jump.mp3" );
    jumpSound.setVolume( 0.25f );

    hitSound.loadSound( "assets/sounds/death.wav" );
    hitSound.setVolume( 0.5f );

}

void Player::reset()
{
    auto iter = std::ranges::find( characters, currentCharacter );
    if ( iter == characters.end() || ++iter == characters.end() )
    {
        iter = characters.begin();
    }
    currentCharacter = *iter;

    currentCharacter->setAnimation( "Idle" );
    setVelocity( glm::vec2 { 0 } );
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
    case State::LeftWallJump:
    case State::RightWallJump:
        doWallJump( deltaTime );
        break;
    }

    // Update player position.
    transform.translate( glm::vec2 { velocity.x, -velocity.y } * deltaTime );

    // Dampen x velocity
    velocity.x = velocity.x / ( 1.0f + xDampen * deltaTime );

    // Update jump timer.
    jumpTimer += deltaTime;

    if ( currentCharacter )
    {
        currentCharacter->update( deltaTime );
    }
}

void Player::draw( sr::Image& image ) const noexcept
{
    if ( currentCharacter )
    {
        currentCharacter->draw( image, transform );
    }

#if _DEBUG
    // Draw the current state of the player.
    auto pos = transform.getPosition() - glm::vec2 { 12, 50 };
    image.drawText( Font::Default, static_cast<int>( pos.x ), static_cast<int>( pos.y ), stateToString[state], Color::White );

    // Draw the AABB of the player
    image.drawAABB( getAABB(), Color::Red, BlendMode::Disable, FillMode::WireFrame );
#endif
}

void Player::setState( State newState )
{
    if ( state != newState )
    {
        endState( state, newState );
        startState( state, newState );
        state = newState;
    }
}

void Player::setCharacter( size_t characterId )
{
    characterId      = characterId % characters.size();
    currentCharacter = characters[characterId];
    currentCharacter->setAnimation( "Idle" );
}

void Player::startState( State oldState, State newState )
{
    switch ( newState )
    {
    case State::Idle:
        currentCharacter->setAnimation( "Idle" );
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
        jumpSound.replay();
        break;
    case State::Hit:
        currentCharacter->setAnimation( "Hit" );
        // Move the anchor point to the center of the player.
        transform.setAnchor( { 16, 16 } );
        velocity.y = jumpSpeed;
        hitSound.replay();
        break;
    case State::DoubleJump:
        currentCharacter->setAnimation( "Double Jump" );
        velocity.y    = jumpSpeed;
        canDoubleJump = false;
        jumpSound.replay();
        break;
    case State::Falling:
        currentCharacter->setAnimation( "Fall" );
        // Set the fall timer according to the previous state.
        switch ( oldState )
        {
        case State::Idle:
        case State::Run:
        case State::LeftWallJump:
        case State::RightWallJump:
            fallTimer = 0.0f;
            break;
        default:
            fallTimer = coyoteTime;
        }
        break;
    case State::LeftWallJump:
        transform.setScale( { -1, 1 } );
        currentCharacter->setAnimation( "Wall Jump" );
        break;
    case State::RightWallJump:
        transform.setScale( { 1, 1 } );
        currentCharacter->setAnimation( "Wall Jump" );
        break;
    case State::Dead:
        std::cout << "Player died..." << std::endl;
        break;
    }
}

void Player::endState( State oldState, State newState )
{
}

float Player::doHorizontalMovement( float deltaTime )
{
    const float horizontal = Input::getAxis( "Horizontal" ) * accel * deltaTime;

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
    velocity.x += horizontal;

    if ( jumpTimer < jumpBuffer || Input::getButtonDown( "Jump" ) )
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
    velocity.x += horizontal;

    // Apply gravity
    velocity.y -= gravity * deltaTime;

    if ( Input::getButtonDown( "Jump" ) )
    {
        if ( canDoubleJump )
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

    // Rotate the player while falling.
    transform.rotate( std::numbers::pi_v<float> * deltaTime );

    // If player is off the bottom of the screen...
    if ( transform.getPosition().y > 1000.0f )
    {
        setState( State::Dead );
    }
}

void Player::doDoubleJump( float deltaTime )
{
    const float horizontal = doHorizontalMovement( deltaTime );
    velocity.x += horizontal;

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
    velocity.x += horizontal;

    velocity.y -= gravity * deltaTime;

    if ( Input::getButtonDown( "Jump" ) )
    {
        if ( fallTimer < coyoteTime )  // Allow jumping for a short time after starting to fall.
            setState( State::Jump );
        else if ( canDoubleJump )
            setState( State::DoubleJump );
        else
            jumpTimer = 0.0f;  // Allow the player to jump if landing after a short time.
    }

    fallTimer += deltaTime;
}

void Player::doWallJump( float deltaTime )
{
    const float horizontal = doHorizontalMovement( deltaTime );
    velocity.x += horizontal;

    // Apply gravity
    velocity.y -= gravity * deltaTime;

    // Clamp gravity.
    velocity.y = std::max( velocity.y, -gravity * deltaTime * 3.0f );

    if ( Input::getButtonDown( "Jump" ) )
    {
        velocity.x = state == State::LeftWallJump ? wallJumpSpeed : -wallJumpSpeed;
        setState( State::Jump );
    }
}
