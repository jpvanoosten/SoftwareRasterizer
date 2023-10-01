#include <Math/Camera3D.hpp>

using namespace Math;


void Camera::setProjection( float fov, float aspect, float near, float far )
{
    projectionMatrix = glm::perspective( fov, aspect, near, far );
    viewProjectionDirty = true;
}

void Camera::setTranslation( const glm::vec3& pos )
{
    translation = pos;
    viewDirty   = true;
}

void Camera::setRotation( const glm::quat& rot )
{
    rotation = rot;
    viewDirty = true;
}

void Camera::translate( const glm::vec3& t, Space space )
{
    switch ( space )
    {
    case Space::Local:
        translation += rotation * t;
        break;
    case Space::World:
        translation += t;
        break;
    }

    viewDirty = true;
}

void Camera::rotate( const glm::quat& rot )
{
    rotation = rotation * rot;
    viewDirty = true;
}

void Camera::rotateX( float pitch )
{
    rotation = rotation * glm::angleAxis( pitch, glm::vec3{ 1, 0, 0 } );
    viewDirty = true;
}

void Camera::rotateY( float yaw )
{
    rotation = rotation * glm::angleAxis( yaw, glm::vec3 { 0, 1, 0 } );
    viewDirty = true;
}

void Camera::rotateZ( float roll )
{
    rotation = rotation * glm::angleAxis( roll, glm::vec3 { 0, 0, 1 } );
    viewDirty = true;
}

const glm::mat4& Camera::getViewMatrix() const noexcept
{
    if (viewDirty)
    {
        // TODO: Compute view matrix
        viewDirty = false;
    }

    return viewMatrix;
}
