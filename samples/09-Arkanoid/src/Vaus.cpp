#include <Ball.hpp>
#include <Vaus.hpp>

#include <Graphics/Font.hpp>
#include <Graphics/Input.hpp>

#include <map>
#include <string>

using namespace Graphics;
using namespace Math;

static std::map<Vaus::State, std::string> stateToString = {
    { Vaus::State::Wait, "Wait" },
    { Vaus::State::Appear, "Appear" },
    { Vaus::State::Default, "Default" },
    { Vaus::State::ToLaser, "To Laser" },
    { Vaus::State::Laser, "Laser" },
    { Vaus::State::Enlarge, "Enlarge" },
    { Vaus::State::ExplodeStage1, "Explode 1" },
    { Vaus::State::ExplodeStage2, "Explode 2" },
    { Vaus::State::Dead, "Dead" },

};

inline Circle operator*( const Camera2D& camera, const Circle& c )
{
    return { camera * c.center, camera.getZoom() * c.radius };
}

AABB operator*( const Camera2D& camera, const AABB& aabb )
{
    return { camera * aabb.min, camera * aabb.max };
}

Vaus::Vaus() = default;

Vaus::Vaus( const std::shared_ptr<Graphics::SpriteSheet>& spriteSheet )
: aabb { { 4, 0, 0 }, { 28, 8, 0 } }
, enlargeAABB { { 4, 0, 0 }, { 44, 8, 0 } }
, leftCircle { { 0, 0 }, 4 }
, rightCircle( { 0, 0 }, 4 )
{
    // The animation frames in the sprite sheet for the various modes.
    {
        constexpr int frames[] { 0, 1, 2, 3, 4 };
        appearMode = SpriteAnim { spriteSheet, FPS, frames };
    }
    {
        constexpr int frames[] { 6, 7, 8, 9, 10, 11, 11 };
        defaultMode = SpriteAnim { spriteSheet, FPS, frames };
    }
    {
        constexpr int frames[] { 12, 13, 14, 15, 16, 17, 17 };
        enlargeMode = SpriteAnim { spriteSheet, FPS, frames };
    }
    {
        constexpr int frames[] { 18, 19, 20, 21, 22, 23, 24, 25, 26 };
        toLaserMode = SpriteAnim { spriteSheet, FPS, frames };
    }
    {
        constexpr int frames[] { 27, 28, 29, 30, 31, 32 };
        laserMode = SpriteAnim { spriteSheet, FPS, frames };
    }
    {
        constexpr int frames[] { 36, 37, 38 };
        explode1 = SpriteAnim { spriteSheet, FPS, frames };
    }
    {
        constexpr int frames[] { 39, 40, 41, 42 };
        explode2 = SpriteAnim { spriteSheet, FPS, frames };
    }

    // Set the anchor point to the center of the paddle (depending on mode)
    transform.setAnchor( { 16, 4 } );
    enlargeTransform.setAnchor( { 24, 4 } );
    explosionTransform.setAnchor( { 24, 12 } );
}

void Vaus::update( float deltaTime )
{
    switch ( state )
    {
    case State::Wait:
        doWait( deltaTime );
        break;
    case State::Appear:
        doAppear( deltaTime );
        break;
    case State::Default:
        doDefault( deltaTime );
        break;
    case State::ToLaser:
        doToLaser( deltaTime );
        break;
    case State::Laser:
        doLaser( deltaTime );
        break;
    case State::Enlarge:
        doEnlarge( deltaTime );
        break;
    case State::Dead:
        doDead( deltaTime );
        break;
    case State::ExplodeStage1:
    case State::ExplodeStage2:
        doExplosion( deltaTime );
        break;
    }

#if _DEBUG
    if ( Input::getKeyDown( KeyCode::D1 ) )
    {
        setState( State::Appear );
    }
    else if ( Input::getKeyDown( KeyCode::D2 ) )
    {
        setState( State::ToLaser );
    }
    else if ( Input::getKeyDown( KeyCode::D3 ) )
    {
        setState( State::Enlarge );
    }
    else if ( Input::getKeyDown( KeyCode::D4 ) )
    {
        setState( State::ExplodeStage1 );
    }
#endif
}

void Vaus::draw( Graphics::Image& image )
{
    const SpriteAnim*  sprite = nullptr;
    const Transform2D* t      = &transform;

    switch ( state )
    {
    case State::Wait:
        // When waiting, no sprite is rendered.
        break;
    case State::Appear:
        sprite = &appearMode;
        break;
    case State::Default:
        sprite = &defaultMode;
        break;
    case State::ToLaser:
        sprite = &toLaserMode;
        break;
    case State::Laser:
        sprite = &laserMode;
        break;
    case State::Enlarge:
        sprite = &enlargeMode;
        t      = &enlargeTransform;
        break;
    case State::ExplodeStage1:
        sprite = &explode1;
        break;
    case State::ExplodeStage2:
        sprite = &explode2;
        t      = &explosionTransform;
        break;
    }

    if ( sprite )
    {
        Sprite normalSprite = *sprite;
        Sprite blackSprite  = *sprite;

        blackSprite.setColor( Color::Black );

        int x = t->getPosition().x - t->getAnchor().x;
        int y = t->getPosition().y - t->getAnchor().y;

        // Draw the shadow.
        image.drawSprite( blackSprite, x + 4, y + 4 );
        // Draw the regular sprite.
        image.drawSprite( normalSprite, x, y );
    }

#if _DEBUG
    image.drawAABB( getAABB(), Color::Yellow, {}, FillMode::WireFrame );
    image.drawCircle( leftCircle, Color::Yellow, {}, FillMode::WireFrame );
    image.drawCircle( rightCircle, Color::Yellow, {}, FillMode::WireFrame );

    // Draw vaus's current state.
    auto pos = transform.getPosition() - glm::vec2 { 20, 20 };
    image.drawText( Font::Default, stateToString[state], pos.x, pos.y, Color::White );
#endif
}

void Vaus::setPosition( const glm::vec2& pos )
{
    transform.setPosition( pos );
    enlargeTransform.setPosition( pos );
    explosionTransform.setPosition( pos );

    const glm::vec2 paddleWidth { getExtent(), 0 };

    leftCircle.center  = pos - paddleWidth;
    rightCircle.center = pos + paddleWidth;
}

const glm::vec2& Vaus::getPosition() const
{
    return transform.getPosition();
}

float Vaus::getExtent() const noexcept
{
    float width = 12.0f;

    switch ( state )
    {
    case State::Enlarge:
        width = 20.0f;
        break;
    }

    return width;
}

Math::AABB Vaus::getAABB() const
{
    // Get the AABB depending on the current state of Vaus.
    switch ( state )
    {
    case State::Enlarge:
        return enlargeTransform * enlargeAABB;
    default:
        return transform * aabb;
    }
}

std::optional<Physics::HitInfo> Vaus::collidesWith( const Ball& ball ) const
{
    const auto& c = ball.getCircle();

    if ( const auto hit = Physics::collidesWith( getAABB(), ball.getCircle(), ball.getVelocity() ) )
    {
        return hit;
    }
    if ( const auto hit = Physics::collidesWith( leftCircle, c ) )
    {
        return hit;
    }
    if ( const auto hit = Physics::collidesWith( rightCircle, c ) )
    {
        return hit;
    }

    return {};
}

void Vaus::setState( State newState )
{
    if ( state != newState )
    {
        beginState( newState );

        const State oldState = state;
        state                = newState;

        endState( oldState );
    }
}

Vaus::State Vaus::getState() const noexcept
{
    return state;
}

void Vaus::updateControls( float deltaTime )
{
    auto pos = getPosition();

    pos.x += Input::getAxis( "Horizontal" ) * speed * deltaTime;

    if ( pos.x - getExtent() < 8.0f )
        pos.x = getExtent() + 8.0f;
    else if ( pos.x >= 216 )
        pos.x = 216 - getExtent();

    setPosition( pos );
}

void Vaus::beginState( State newState )
{
    switch ( newState )
    {
    case State::Wait:
        break;
    case State::Appear:
        appearMode.reset();
        break;
    case State::Default:
        defaultMode.reset();
        break;
    case State::ToLaser:
        toLaserMode.reset();
        break;
    case State::Laser:
        laserMode.reset();
        break;
    case State::Enlarge:
        enlargeMode.reset();
        break;
    case State::Dead:
        break;
    case State::ExplodeStage1:
        explode1.reset();
        break;
    case State::ExplodeStage2:
        explode2.reset();
        break;
    }
}

void Vaus::endState( State oldState )
{
    switch ( oldState )
    {
    case State::Wait:
        break;
    case State::Appear:
        break;
    case State::Default:
        break;
    case State::ToLaser:
        break;
    case State::Laser:
        break;
    case State::Enlarge:
        break;
    case State::ExplodeStage1:
        break;
    case State::ExplodeStage2:
        break;
    case State::Dead:
        break;
    }
}

void Vaus::doWait( float deltaTime )
{
    // Do nothing until Vaus is transitioned to the appear state.
}

void Vaus::doAppear( float deltaTime )
{
    // Update the appear animation.
    appearMode.update( deltaTime );
    if ( appearMode.isDone() )
    {
        // If the appear animation is finished playing,
        // transition to the default state.
        setState( State::Default );
    }
}

void Vaus::doDefault( float deltaTime )
{
    updateControls( deltaTime );
    // Update the default animation.
    defaultMode.update( deltaTime );
}

void Vaus::doToLaser( float deltaTime )
{
    updateControls( deltaTime );

    // Update the laser animation.
    toLaserMode.update( deltaTime );
    if ( toLaserMode.isDone() )
    {
        // Transition to the laser state.
        setState( State::Laser );
    }
}

void Vaus::doLaser( float deltaTime )
{
    updateControls( deltaTime );
    // Update the laser animation.
    laserMode.update( deltaTime );
}

void Vaus::doEnlarge( float deltaTime )
{
    updateControls( deltaTime );
    // Update the enlarge animation.
    enlargeMode.update( deltaTime );
}

void Vaus::doExplosion( float deltaTime )
{
    switch ( state )
    {
    case State::ExplodeStage1:
        explode1.update( deltaTime );
        if ( explode1.isDone() )
        {
            setState( State::ExplodeStage2 );
        }
        break;
    case State::ExplodeStage2:
        explode2.update( deltaTime );
        if ( explode2.isDone() )
        {
            setState( State::Wait );
        }
        break;
    }
}

void Vaus::doDead( float deltaTime )
{
    // Go directly to explosion state.
    setState( State::ExplodeStage1 );
}
