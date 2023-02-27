#include <Graphics/GamePad.hpp>

#include <algorithm>

namespace Graphics
{
float ApplyLinearDeadZone( float value, float maxValue, float deadZoneSize = 0.0f ) noexcept
{
    if ( value < -deadZoneSize )
    {
        // Increase negative values to remove the dead-zone discontinuity.
        value += deadZoneSize;
    }
    else if ( value > deadZoneSize )
    {
        // Decrease positive values to remove the dead-zone discontinuity.
        value -= deadZoneSize;
    }
    else
    {
        // Values inside the dead-zone come out zero.
        return 0;
    }

    // Scale into 0-1 range.
    const float scaledValue = value / ( maxValue - deadZoneSize );
    return std::clamp( scaledValue, -1.0f, 1.f );
}

void ApplyStickDeadZone( float x, float y, DeadZone deadZoneMode, float maxValue, float deadZoneSize, float& resultX,
                         float& resultY ) noexcept
{
    switch ( deadZoneMode )
    {
    case DeadZone::IndependentAxis:
        resultX = ApplyLinearDeadZone( x, maxValue, deadZoneSize );
        resultY = ApplyLinearDeadZone( y, maxValue, deadZoneSize );
        break;
    case DeadZone::Circular:
    {
        const float dist   = sqrtf( x * x + y * y );
        const float wanted = ApplyLinearDeadZone( dist, maxValue, deadZoneSize );

        const float scale = ( wanted > 0.f ) ? ( wanted / dist ) : 0.f;

        resultX = std::clamp( x * scale, -1.0f, 1.f );
        resultY = std::clamp( y * scale, -1.0f, 1.0f );
    }
    break;
    case DeadZone::None:
        resultX = ApplyLinearDeadZone( x, maxValue, 0 );
        resultY = ApplyLinearDeadZone( y, maxValue, 0 );
        break;
    }
}
}  // namespace Graphics
