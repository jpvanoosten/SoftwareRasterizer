#include <Math/Camera2D.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

using namespace Math;

Camera2D::Camera2D( const glm::vec2& position, const glm::vec2& size ) noexcept
: m_Position { position }
, m_Size { size }
{}

void Camera2D::translate( const glm::vec2& translation, Space space ) noexcept
{
    switch ( space )
    {
    case Space::Local:  // Translation is applied in the camera's local (rotated) space.
    {
        // Build a rotation matrix to rotate the translation vector.
        const float c = glm::cos( m_Rotation );
        const float s = glm::sin( m_Rotation );

        // GLM is column-major.
        const glm::mat3 rot {
            c, s, 0,
            -s, c, 0,
            0, 0, 1
        };

        // Compute the rotated translation vector.
        const glm::vec2 t = rot * glm::vec3 { translation, 0 };

        // Translate the camera by the rotated translation vector.
        setPosition( m_Position + t );
    }
    break;
    case Space::World:  // Translation is in the world coordinate system (not rotated into the camera's local space).
        setPosition( m_Position + translation );
        break;
    }
}

const glm::mat3& Camera2D::getTransform() const noexcept
{
    if ( m_ViewMatrixDirty )
    {
        const auto translate1 = glm::translate( glm::vec3{ -m_Position, 1.0f } );
        const auto translate2 = glm::translate( glm::vec3 { m_Size / 2.0f, 1.0f } );
        const auto rotate    = glm::rotate( -m_Rotation, glm::vec3 { 0, 0, 1 } );
        const auto scale      = glm::scale( glm::vec3 { m_Zoom, m_Zoom, 1 } );

        const auto transform = translate2 * rotate * scale * translate1;

        m_ViewMatrix = {
            transform[0][0], transform[0][1], transform[0][3],
            transform[1][0], transform[1][1], transform[1][3],
            transform[3][0], transform[3][1], transform[3][3]
        };

        m_ViewMatrixDirty = false;
    }

    return m_ViewMatrix;
}
