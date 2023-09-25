#pragma once

#include "Space.hpp"

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>

namespace Math
{
class Camera final
{
public:
    Camera()                    = default;
    Camera( const Camera& )     = default;
    Camera( Camera&& ) noexcept = default;
    ~Camera()                   = default;

    Camera& operator=( const Camera& )     = default;
    Camera& operator=( Camera&& ) noexcept = default;

    /// <summary>
    /// Set the camera's projection matrix.
    /// </summary>
    /// <param name="fov">Vertical field of view (in radians).</param>
    /// <param name="aspect">Aspect ratio of the screen (width/height).</param>
    /// <param name="near">Distance to the near clipping plane.</param>
    /// <param name="far">Distance to the far clipping plane.</param>
    void setProjection( float fov, float aspect, float near, float far );

    /// <summary>
    /// Set the view matrix of the camera.
    /// </summary>
    /// <param name="eye">The eye position (in world space).</param>
    /// <param name="target">The target position (in world space).</param>
    /// <param name="up">The camera's up vector (should be perpendicular to (target - eye))</param>
    void setView( const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up = glm::vec3 { 0, 1, 0 } );

    /// <summary>
    /// Set the translation of the camera.
    /// </summary>
    /// <param name="pos"></param>
    void setTranslation( const glm::vec3& pos );

    /// <summary>
    /// Set the camera's rotation.
    /// </summary>
    /// <param name="rot">The rotation quaternion.</param>
    void setRotation( const glm::quat& rot );

    /// <summary>
    /// Add translation to the camera.
    /// </summary>
    /// <param name="translation">The amount to add to the camera's position.</param>
    /// <param name="space">(optional) The space to apply the translation (Default: Local)</param>
    void addTranslation( const glm::vec3& translation, Space space = Space::Local );

    /// <summary>
    /// Add rotation to the camera.
    /// </summary>
    /// <param name="rot">The rotation quaternion to add.</param>
    void addRotation( const glm::quat& rot );

private:
    float fov;     // Vertical field of view.
    float aspect;  // Aspect ratio.
    float near;    // Near clipping plane.
    float far;     // far clipping plane.

    glm::vec3 translation { 0 };  // World-space position.
    glm::quat rotation {};        // Rotation quaternion.

    glm::mat4 viewMatrix;
    glm::mat4 inverseViewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 inverseProjectionMatrix;

    bool viewDirty                    = true;
    bool inverseViewMatrixDirty       = true;
    bool inverseProjectionMatrixDirty = true;
};
}  // namespace Math
