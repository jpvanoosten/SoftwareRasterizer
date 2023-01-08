#include <Player.hpp>

#include <Font.hpp>
#include <Input.hpp>
#include <ResourceManager.hpp>
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
    { Player::State::LeftWallJump, "Left Wall Jump" },
    { Player::State::RightWallJump, "Right Wall Jump" }
};

using namespace sr;

Character createCharacter( const std::filesystem::path& basePath )
{
    Character character;

    // Load the sprite sheets for the character.
    const auto doubleJump = ResourceManager::loadSpriteSheet( basePath / "Double Jump (32x32).png", 32, 32, BlendMode::AlphaBlend );
    const auto fall       = ResourceManager::loadSpriteSheet( basePath / "Fall (32x32).png", 32, 32, BlendMode::AlphaBlend );
    const auto hit        = ResourceManager::loadSpriteSheet( basePath / "Hit (32x32).png", 32, 32, BlendMode::AlphaBlend );
    const auto idle       = ResourceManager::loadSpriteSheet( basePath / "Idle (32x32).png", 32, 32, BlendMode::AlphaBlend );
    const auto jump       = ResourceManager::loadSpriteSheet( basePath / "Jump (32x32).png", 32, 32, BlendMode::AlphaBlend );
    const auto run        = ResourceManager::loadSpriteSheet( basePath / "Run (32x32).png", 32, 32, BlendMode::AlphaBlend );
    const auto wallJump   = ResourceManager::loadSpriteSheet( basePath / "Wall Jump (32x32).png", 32, 32, BlendMode::AlphaBlend );

    // Add the sprite animations for the character.
    character.addAnimation( "Double Jump", SpriteAnim { doubleJump, 20 } );
    character.addAnimation( "Fall", SpriteAnim { fall, 20 } );
    character.addAnimation( "Hit", SpriteAnim { hit, 20 } );
    character.addAnimation( "Idle", SpriteAnim { idle, 20 } );
    character.addAnimation( "Jump", SpriteAnim { jump, 20 } );
    character.addAnimation( "Run", SpriteAnim { run, 20 } );
    character.addAnimation( "Wall Jump", SpriteAnim { wallJump, 20 } );

    return character;
}

Player::Player( const Math::Transform2D& transform )
: transform { transform }
, aabb { { 7, 5, 0 }, { 25, 32, 0 } }  // Player is 32x32 pixels, but the AABB should be slightly smaller.
, topAABB { { 7, 3, 0 }, { 25, 5, 0 } }
, bottomAABB { { 7, 30, 0 }, { 25, 32, 0 } }
, leftAABB { { 5, 5, 0 }, { 7, 29, 0 } }
, rightAABB { { 25, 5, 0 }, { 27, 29, 0 } }
{
    characters.emplace_back( createCharacter( "assets/Pixel Adventure/Main Characters/Mask Dude" ) );
    characters.emplace_back( createCharacter( "assets/Pixel Adventure/Main Characters/Ninja Frog" ) );
    characters.emplace_back( createCharacter( "assets/Pixel Adventure/Main Characters/Pink Man" ) );
    characters.emplace_back( createCharacter( "assets/Pixel Adventure/Main Characters/Virtual Guy" ) );

    currentCharacter = characters.begin();

    currentCharacter->setAnimation( "Idle" );
}

Player::Player( const Player& copy )
: characters { copy.characters }
, currentCharacter { characters.end() }
, transform { copy.transform }
, aabb { copy.aabb }
, topAABB { copy.topAABB }
, bottomAABB { copy.bottomAABB }
, leftAABB { copy.leftAABB }
, rightAABB { copy.rightAABB }
, state { copy.state }
, xDampen { copy.xDampen }
, canDoubleJump { copy.canDoubleJump }
, velocity { copy.velocity }
{}

Player::Player( Player&& other ) noexcept
: characters { std::move( other.characters ) }
, currentCharacter { characters.end() }
, transform { other.transform }
, aabb { other.aabb }
, topAABB { other.aabb }
, bottomAABB { other.bottomAABB }
, leftAABB { other.leftAABB }
, rightAABB { other.rightAABB }
, state { other.state }
, xDampen { other.xDampen }
, canDoubleJump { other.canDoubleJump }
, velocity { other.velocity }
{}

Player& Player::operator=( const Player& copy )
{
    if ( this == &copy )
        return *this;

    characters       = copy.characters;
    currentCharacter = characters.end();
    transform        = copy.transform;
    aabb             = copy.aabb;
    topAABB          = copy.topAABB;
    bottomAABB       = copy.bottomAABB;
    leftAABB         = copy.leftAABB;
    rightAABB        = copy.rightAABB;
    state            = copy.state;
    xDampen          = copy.xDampen;
    canDoubleJump    = copy.canDoubleJump;
    velocity         = copy.velocity;

    return *this;
}

Player& Player::operator=( Player&& other ) noexcept
{
    if ( this == &other )
        return *this;

    characters       = std::move( other.characters );
    currentCharacter = characters.end();
    transform        = other.transform;
    aabb             = other.aabb;
    topAABB          = other.topAABB;
    bottomAABB       = other.bottomAABB;
    leftAABB         = other.leftAABB;
    rightAABB        = other.rightAABB;
    state            = other.state;
    xDampen          = other.xDampen;
    canDoubleJump    = other.canDoubleJump;
    velocity         = other.velocity;

    return *this;
}

void Player::reset()
{
    if ( ++currentCharacter == characters.end() )
    {
        currentCharacter = characters.begin();
    }

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

    // Clamp velocity.
    //    velocity.x = std::clamp( velocity.x, -maxSpeed, maxSpeed );

    // Update player position.
    transform.translate( glm::vec2 { velocity.x, -velocity.y } * deltaTime );

    // Dampen x velocity
    velocity.x = velocity.x / ( 1.0f + xDampen * deltaTime );

    if ( currentCharacter != characters.end() )
    {
        currentCharacter->update( deltaTime );
    }
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

    // Draw the AABB of the player
    image.drawAABB( getAABB(), Color::Red, BlendMode::Disable, FillMode::WireFrame );
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

void Player::setCharacter( size_t characterId )
{
    characterId      = characterId % characters.size();
    currentCharacter = characters.begin() + characterId;
    currentCharacter->setAnimation( "Idle" );
}

void Player::startState( State newState )
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

void Player::endState( State oldState )
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
    velocity.x += horizontal;

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

    if ( canDoubleJump && Input::getButtonDown( "Jump" ) )
    {
        setState( State::DoubleJump );
    }
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
