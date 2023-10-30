#pragma once

#include <Math/Camera3D.hpp>

class CameraController
{
public:

    explicit CameraController( const glm::vec3& pos = glm::vec3{0}, float pitch = 0.0f, float yaw = 0.0f );

    /// <summary>
    /// Set the perspective projection matrix of the camera.
    /// </summary>
    /// <param name="fovy">Vertical field of view (in degrees).</param>
    /// <param name="aspect">Aspect ratio (width / height)</param>
    /// <param name="near">Distance to the near clipping plane.</param>
    /// <param name="far">Distance to the far clipping plane.</param>
    void setPerspective( float fovy, float aspect, float near = 0.1f, float far = 100.0f );

    /// <summary>
    /// Reset the camera back to it's initial state.
    /// </summary>
    void reset();

    void update( float deltaTime );

    const glm::mat4& getViewMatrix() const noexcept;

    const glm::mat4& getProjectionMatrix() const noexcept;

    const glm::mat4& getViewProjectionMatrix() const noexcept;

private:
    Math::Camera camera{};

    // Keep camera initial state in a struct to it is easy to reset the camera back to the initial state.
    struct State
    {
        glm::vec3 pos { 0 };
        float     pitch = 0.0f;
        float     yaw   = 0.0f;
    } state, initialState;

    static constexpr inline float rotationSpeed = 5.0f;  // Degrees per second.
    static constexpr inline float movementSpeed = 10.0f; // Units per second.
};