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
    /// Update the rotation to face the target.
    /// </summary>
    /// <param name="eye">The eye position.</param>
    /// <param name="target">The target point to look at.</param>
    /// <param name="up">(optional) The global "up" vector. Default: (0, 1, 0).</param>
    void lookAt( const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up = {0, 1, 0} );

    /// <summary>
    /// Add translation to the camera.
    /// </summary>
    /// <param name="translation">The amount to add to the camera's position.</param>
    /// <param name="space">(optional) The space to apply the translation (Default: Local)</param>
    void translate( const glm::vec3& translation, Space space = Space::Local );

    /// <summary>
    /// Add rotation to the camera.
    /// </summary>
    /// <param name="rot">The rotation quaternion to add.</param>
    void rotate( const glm::quat& rot );

    /// <summary>
    /// Rotate about the X-axis (pitch).
    /// </summary>
    /// <param name="pitch">The pitch (in radians).</param>
    void rotateX( float pitch );

    /// <summary>
    /// Rotate about the Y-axis (yaw).
    /// </summary>
    /// <param name="yaw">The yaw (in radians).</param>
    void rotateY( float yaw );

    /// <summary>
    /// Rotate about the Z-axis (roll).
    /// </summary>
    /// <param name="roll">The roll (in radians).</param>
    void rotateZ( float roll );

    /// <summary>
    /// Get the camera's view matrix.
    /// </summary>
    /// <returns>The view matrix of the camera.</returns>
    const glm::mat4& getViewMatrix() const noexcept;

    /// <summary>
    /// Get the camera's projection matrix.
    /// </summary>
    /// <returns>The camera's projection matrix.</returns>
    const glm::mat4& getProjectionMatrix() const noexcept;

    /// <summary>
    /// Get the concatenation of the view and projection matrices.
    /// </summary>
    /// <returns></returns>
    const glm::mat4& getViewProjectionMatrix() const noexcept;

private:
    void updateViewMatrix();
    void updateViewProjectionMatrix();

    glm::vec3 translation { 0 };  // World-space position.
    glm::quat rotation {};        // Rotation quaternion.

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 viewProjectionMatrix;

    bool viewDirty           = true;
    bool viewProjectionDirty = true;
};
}  // namespace Math
