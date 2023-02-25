#include <Math/Camera2D.hpp>

#include <glm/gtc/matrix_transform.hpp>
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
        auto transform = glm::translate( glm::rotate( glm::scale( glm::translate( glm::mat4 { 1.0f }, glm::vec3 { -m_Position + m_Origin, 0 } ), glm::vec3 { m_Zoom, m_Zoom, 1 } ), -m_Rotation, glm::vec3 { 0, 0, 1 } ), glm::vec3 { -m_Origin, 0 } );

        m_Transform = {
            transform[0][0], transform[0][1], transform[0][3],
            transform[1][0], transform[1][1], transform[1][3],
            transform[3][0], transform[3][1], transform[3][3]
        };

        m_TransformDirty = false;
    }

    return m_Transform;
}
