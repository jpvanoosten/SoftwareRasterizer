#include <Box.hpp>

Box::Box( const Character& boxCharacter, int hitPoints, const Math::AABB& collision )
: aabb { collision }
, hitPoints { hitPoints }
, character { &boxCharacter }
{
}

void Box::setState( State newState ) {}
