#include "ListenerImpl.hpp"

using namespace Audio;

void ListenerImpl::setPosition( const glm::vec3& pos )
{
    ma_engine_listener_set_position( engine, index, pos.x, pos.y, pos.z );
}

glm::vec3 ListenerImpl::getPosition() const
{
    auto pos = ma_engine_listener_get_position( engine, index );
    return { pos.x, pos.y, pos.z };
}

void ListenerImpl::setDirection( const glm::vec3& dir )
{
    ma_engine_listener_set_direction( engine, index, dir.x, dir.y, dir.z );
}

glm::vec3 ListenerImpl::getDirection() const
{
    auto dir = ma_engine_listener_get_direction( engine, index );
    return { dir.x, dir.y, dir.z };
}

void ListenerImpl::setUp( const glm::vec3& up )
{
    ma_engine_listener_set_world_up( engine, index, up.x, up.y, up.z );
}

glm::vec3 ListenerImpl::getUp() const
{
    auto up = ma_engine_listener_get_world_up( engine, index );
    return { up.x, up.y, up.z };
}

void ListenerImpl::setCone( float innerConeAngle, float outerConeAngle, float outerGain )
{
    ma_engine_listener_set_cone( engine, index, innerConeAngle, outerConeAngle, outerGain );
}

void ListenerImpl::getCone( float& innerConeAngle, float& outerConeAngle, float& outerGain ) const
{
    ma_engine_listener_get_cone( engine, index, &innerConeAngle, &outerConeAngle, &outerGain );
}
