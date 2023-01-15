#include <Box.hpp>

using namespace sr;

Box::Box( int hitPoints )
: Character()
, hitPoints { hitPoints }
{}

Box::Box( const Box& copy )
: Character( copy )
, aabb { copy.aabb }
, transform { copy.transform }
, hitPoints { copy.hitPoints }
{}

Box::Box( Box&& other ) noexcept
: Character( std::move( other ) )
, aabb { other.aabb }
, transform { other.transform }
, hitPoints { other.hitPoints }
{}

Box& Box::operator=( const Box& copy )
{
    if ( this == &copy )
        return *this;

    aabb      = copy.aabb;
    transform = copy.transform;
    hitPoints = copy.hitPoints;
    Character::operator=( copy );

    return *this;
}

Box& Box::operator=( Box&& other ) noexcept
{
    if ( this == &other )
        return *this;

    aabb      = other.aabb;
    hitPoints = other.hitPoints;
    transform = other.transform;
    Character::operator=( std::move( other ) );

    return *this;
}

void Box::setPosition( const glm::vec2& position )
{
    // A box sprite is 28x24 pixels, but the visual box is 20x20 pixels.
    // Position is the bottom-center point of the box.
    aabb      = Math::AABB { glm::vec3 { position.x - 10, position.y - 20, 0 }, glm::vec3 { position.x + 9, position.y, 0 } };
    transform = Math::Transform2D { position };
    transform.setAnchor( glm::vec2 { 14, 22 } );
}

void Box::update( float deltaTime )
{
    Character::update( deltaTime );

    switch ( state )
    {
    case State::Idle:
        updateIdle( deltaTime );
        break;
    case State::Hit:
        updateHit( deltaTime );
        break;
    case State::Break:
        updateBreak( deltaTime );
        break;
    }
}

void Box::draw( sr::Image& image ) const
{
    Character::draw( image, transform );

#if _DEBUG
    image.drawAABB( aabb, Color::Yellow, {}, FillMode::WireFrame );
#endif
}

void Box::setState( State newState )
{
    if ( state != newState )
    {
        endState( state );
        startState( newState );
        state = newState;
    }
}

void Box::startState( State newState )
{
    switch ( newState )
    {
    case State::Idle:
        setAnimation( "Idle" );
        break;
    case State::Hit:
        setAnimation( "Hit" );
        break;
    case State::Break:
        // TODO: Break into pieces.
        break;
    }

}
void Box::endState( State oldState ) {}

void Box::updateIdle( float deltaTime )
{}

void Box::updateHit( float deltaTime )
{
    const auto& hitAnim = getAnimation( "Hit" );
    if ( hitAnim.isDone() )
    {
        // Reduce hit points
        --hitPoints;
        if ( hitPoints > 0 )
            setState( State::Idle );
        else
            setState( State::Break );
    }
}

void Box::updateBreak( float deltaTime )
{}
