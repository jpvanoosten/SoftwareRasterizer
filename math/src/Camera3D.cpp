#include <Math/Camera3D.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

using namespace Math;

void Camera::setProjection( float fov, float aspect, float near, float far )
{
    projectionMatrix    = glm::perspective( fov, aspect, near, far );
    viewProjectionDirty = true;
}

void Camera::setTranslation( const glm::vec3& pos )
{
    translation = pos;
    viewDirty   = true;
}

void Camera::setRotation( const glm::quat& rot )
{
    rotation  = rot;
    viewDirty = true;
}

void Camera::lookAt( const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up )
{
    translation    = eye;
    const auto dir = target - eye;
    if ( length2( dir ) > 0.0f )
        rotation = glm::quatLookAt( glm::normalize( dir ), up );
    else
        rotation = glm::quatLookAt( { 0, 0, -1 }, up );
    
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
    rotation  = rotation * rot;
    viewDirty = true;
}

void Camera::rotateX( float pitch )
{
    rotation  = rotation * glm::angleAxis( pitch, glm::vec3 { 1, 0, 0 } );
    viewDirty = true;
}

void Camera::rotateY( float yaw )
{
    rotation  = rotation * glm::angleAxis( yaw, glm::vec3 { 0, 1, 0 } );
    viewDirty = true;
}

void Camera::rotateZ( float roll )
{
    rotation  = rotation * glm::angleAxis( roll, glm::vec3 { 0, 0, 1 } );
    viewDirty = true;
}

const glm::mat4& Camera::getViewMatrix() const noexcept
{
    const_cast<Camera*>( this )->updateViewMatrix();
    return viewMatrix;
}

const glm::mat4& Camera::getProjectionMatrix() const noexcept
{
    return projectionMatrix;
}

const glm::mat4& Camera::getViewProjectionMatrix() const noexcept
{
    const_cast<Camera*>( this )->updateViewProjectionMatrix();
    return viewProjectionMatrix;
}

void Camera::updateViewMatrix()
{
    if ( viewDirty )
    {
        // Renormalize rotation quaternion.
        rotation     = glm::normalize( rotation );
        const auto T = glm::translate( translation );
        const auto R = glm::toMat4( rotation );

        viewMatrix = glm::inverse( T * R );

        viewDirty = false;
        viewProjectionDirty = true;
    }
}

void Camera::updateViewProjectionMatrix()
{
    if (viewDirty || viewProjectionDirty)
    {
        viewProjectionMatrix = projectionMatrix * getViewMatrix();
        viewProjectionDirty  = false;
    }
}
