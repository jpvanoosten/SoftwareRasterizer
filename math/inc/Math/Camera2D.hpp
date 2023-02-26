#pragma once

#include "Transform2D.hpp"

#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>

namespace Math
{
class Camera2D
{
public:
    enum class Space
    {
        Local,   ///< Translation is applied in the camera's local space.
        World,   ///< Translation is applied in screen world coordinates.
    };

    /// <summary>
    /// Initialize a 2D Camera.
    /// </summary>
    /// <param name="position">The position of the camera (in screen space pixels).</param>
    /// <param name="origin">The origin of the camera's rotation and scaling.</param>
    /// <param name="rotation">The rotation (in radians) of the camera.</param>
    /// <param name="zoom">The zoom of the camera.</param>
    explicit Camera2D( const glm::vec2& position = glm::vec2 { 0 }, const glm::vec2& origin = glm::vec2 { 0 }, float rotation = 0.0f, float zoom = 1.0f ) noexcept;

    /// <summary>
    /// Reset the camera to its default state.
    /// </summary>
    void reset() noexcept
    {
        m_Position       = glm::vec2 { 0 };
        m_Origin         = glm::vec2 { 0 };
        m_Rotation       = 0.0f;
        m_Zoom           = 1.0f;
        m_TransformDirty = true;
    }

    /// <summary>
    /// Set the position of the camera in screen space.
    /// </summary>
    /// <param name="pos">The position of the camera in 2D screen space.</param>
    void setPosition( const glm::vec2& pos ) noexcept
    {
        m_Position       = pos;
        m_TransformDirty = true;
    }

    /// <summary>
    /// Get the current position of the camera.
    /// </summary>
    /// <returns>The current position of the camera.</returns>
    const glm::vec2& getPosition() const noexcept
    {
        return m_Position;
    }

    /// <summary>
    /// Translate the camera's position.
    /// </summary>
    /// <param name="translation">The value to add to the current position of the camera.</param>
    /// <param name="space">The space to apply the translation.</param>
    void translate( const glm::vec2& translation, Space space = Space::Local ) noexcept;

    /// <summary>
    /// Set the origin of the camera.
    /// This is the origin of zooming/rotating of the camera.
    /// </summary>
    /// <param name="origin">The origin of zooming/rotating.</param>
    void setOrigin( const glm::vec2& origin ) noexcept
    {
        m_Origin         = origin;
        m_TransformDirty = true;
    }

    /// <summary>
    /// Get the current origin of the camera.
    /// </summary>
    /// <returns>The current origin of rotation.</returns>
    const glm::vec2& getOrigin() const noexcept
    {
        return m_Origin;
    }

    /// <summary>
    /// Move the origin of the camera.
    /// </summary>
    /// <param name="offset">The offset of the camera origin.</param>
    void moveOrigin( const glm::vec2& offset ) noexcept
    {
        setOrigin( m_Origin + offset );
    }

    /// <summary>
    /// Set the current rotation of the camera (in radians).
    /// </summary>
    /// <param name="rot">The current rotation of the camera (in radians).</param>
    void setRotation( float rot ) noexcept
    {
        m_Rotation       = rot;
        m_TransformDirty = true;
    }

    /// <summary>
    /// Get the current rotation of the camera.
    /// </summary>
    /// <returns>The current rotation of the camera (in radians).</returns>
    float getRotation() const noexcept
    {
        return m_Rotation;
    }

    /// <summary>
    /// Add rotation to the camera.
    /// </summary>
    /// <param name="rot">The rotation to add to the camera.</param>
    void rotate( float rot ) noexcept
    {
        setRotation( m_Rotation + rot );
    }

    /// <summary>
    /// Set the zoom of the camera.
    /// </summary>
    /// <param name="zoom">The zoom of the camera.</param>
    void setZoom( float zoom ) noexcept
    {
        m_Zoom           = zoom;
        m_TransformDirty = true;
    }

    /// <summary>
    /// Get the zoom of the camera.
    /// </summary>
    /// <returns>The zoom of the camera.</returns>
    float getZoom() const noexcept
    {
        return m_Zoom;
    }

    /// <summary>
    /// Increase or decrees the camera zoom.
    /// </summary>
    /// <param name="zoom">The factor to increase or decrease the camera zoom.</param>
    void zoom( float zoom )
    {
        setZoom( m_Zoom + zoom );
    }

    /// <summary>
    /// Get the view matrix of the camera.
    /// </summary>
    /// <returns>The camera's view matrix.</returns>
    const glm::mat3& getTransform() const noexcept;

    /// <summary>
    /// Transform a 2D point by the camera's transform.
    /// </summary>
    /// <param name="point">The point to transform.</param>
    /// <returns></returns>
    glm::vec2 transformPoint( const glm::vec2& point ) const noexcept
    {
        return getTransform() * glm::vec3( point, 1.0f );
    }

    /// <summary>
    /// Transform a 2D vector by the camera's transform.
    /// </summary>
    /// <param name="v">The vector to transform.</param>
    /// <returns></returns>
    glm::vec2 transformVector( const glm::vec2& v ) const noexcept
    {
        return getTransform() * glm::vec3( v, 0.0f );
    }

private:
    glm::vec2 m_Position { 0 };
    glm::vec2 m_Origin { 0 };
    float     m_Rotation { 0.0f };
    float     m_Zoom { 1.0f };

    mutable glm::mat3 m_Transform { 1 };
    mutable bool      m_TransformDirty = true;
};

inline glm::vec2 operator*( const Camera2D& camera, const glm::vec2& point ) noexcept
{
    return camera.getTransform() * glm::vec3( point, 1.0f );
}

inline glm::vec3 operator*( const Camera2D& camera, const glm::vec3& v ) noexcept
{
    return camera.getTransform() * v;
}

inline glm::mat3 operator*( const Camera2D& camera, const Transform2D& transform ) noexcept
{
    return camera.getTransform() * transform.getTransform();
}

}  // namespace Math
