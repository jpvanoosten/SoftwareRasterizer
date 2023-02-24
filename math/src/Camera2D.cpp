#include <Math/Camera2D.hpp>

#include <glm/trigonometric.hpp>

using namespace Math;

Camera2D::Camera2D( const glm::vec2& position, const glm::vec2& origin, float rotation, float zoom ) noexcept
: m_Position { position }
, m_Origin { origin }
, m_Rotation { rotation }
, m_Zoom { zoom }
{}

const glm::mat3& Camera2D::getTransform() const noexcept
{
    if ( m_TransformDirty )
    {
        // TODO: Fix this....
        // Zoom is not taken into consideration yet...
        const float c  = glm::cos( m_Rotation );
        const float s  = glm::sin( m_Rotation );
        const float tx = -m_Origin.x * c - m_Origin.y * s + m_Position.x;
        const float ty = m_Origin.x * s - m_Origin.y * c + m_Position.y;

        m_Transform = {
            c, -s, 0.0f,
            s, c, 0.0f,
            tx, ty, 1.0f
        };

        m_TransformDirty = false;
    }

    return m_Transform;
}

const glm::mat3& Camera2D::getInverseTransform() const noexcept
{
    if ( m_InverseTransformDirty )
    {
        m_InverseTransform      = glm::inverse( getTransform() );
        m_InverseTransformDirty = false;
    }

    return m_InverseTransform;
}
