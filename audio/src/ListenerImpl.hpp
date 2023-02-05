#pragma once

#include "miniaudio.h"

#include <glm/vec3.hpp>

namespace Audio
{
class ListenerImpl
{
public:
    ListenerImpl( uint32_t index, ma_engine* pEngine )
    : index { index }
    , engine { pEngine }
    {}

    ~ListenerImpl() = default;

    void      setPosition( const glm::vec3& pos );
    glm::vec3 getPosition() const;

    void      setDirection( const glm::vec3& dir );
    glm::vec3 getDirection() const;

    void      setUp( const glm::vec3& up );
    glm::vec3 getUp() const;

    void setCone( float innerConeAngle, float outerConeAngle, float outerGain = 1.0f );
    void getCone( float& innerConeAngle, float& outerConeAngle, float& outerGain ) const;

private:
    uint32_t   index;
    ma_engine* engine = nullptr;
};
}  // namespace Audio
