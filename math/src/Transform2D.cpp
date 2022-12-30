#include <Math/Transform2D.hpp>

#include <glm/trigonometric.hpp>

using namespace Math;

Transform2D::Transform2D( const glm::vec2& position, const glm::vec2& scale, float rotation )
: m_Position { position }
, m_Scale { scale }
, m_Rotation { rotation }
{}

const glm::mat3& Transform2D::getTransform() const noexcept
{
    if ( m_TransformDirty )
    {
        const float s   = glm::sin( m_Rotation );
        const float c   = glm::cos( m_Rotation );
        const float sxc = m_Scale.x * c;
        const float syc = m_Scale.y * c;
        const float sxs = m_Scale.x * s;
        const float sys = m_Scale.y * s;
        const float tx  = -m_Anchor.x * sxc - m_Anchor.y * sys + m_Position.x;
        const float ty  = m_Anchor.x * sxs - m_Anchor.y * syc + m_Position.y;

        m_Transform = {
            sxc, -sxs, 0.0f,
            sys, syc, 0.0f,
            tx, ty, 1.0f
        };

        m_TransformDirty = false;
    }

    return m_Transform;
}
