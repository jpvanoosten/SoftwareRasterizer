#include <Player.hpp>

#include <Input.hpp>
#include <SpriteAnim.hpp>

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

Player::Player(const Math::Transform2D& transform)
    : transform{transform}
{
    characters.emplace_back( createCharacter( "assets/Pixel Adventure/Main Characters/Mask Dude" ) );
    characters.emplace_back( createCharacter( "assets/Pixel Adventure/Main Characters/Ninja Frog" ) );
    characters.emplace_back( createCharacter( "assets/Pixel Adventure/Main Characters/Pink Man" ) );
    characters.emplace_back( createCharacter( "assets/Pixel Adventure/Main Characters/Virtual Guy" ) );

    currentCharacter = characters.begin();

    currentCharacter->setAnimation( "Idle" );
}

void Player::reset()
{
    if (++currentCharacter == characters.end())
    {
        currentCharacter = characters.begin();
    }

    currentCharacter->setAnimation( "Idle" );
}

void Player::update( float deltaTime ) noexcept
{
    const float     horizontal = Input::getAxis( "Horizontal" ) * playerSpeed * deltaTime;

    transform.translate( {horizontal, 0.0f} );

    if ( horizontal < 0.0f )
    {
        transform.setScale( { -1, 1 } );
        currentCharacter->setAnimation( "Run" );
    }
    else if (horizontal > 0.0f )
    {
        transform.setScale( { 1, 1 } );
        currentCharacter->setAnimation( "Run" );
    }
    else
    {
        currentCharacter->setAnimation( "Idle" );
    }

    if (currentCharacter != characters.end())
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

void Player::draw( sr::Image& image ) noexcept
{
    if (currentCharacter != characters.end())
    {
        currentCharacter->draw( image, transform );
    }
}
