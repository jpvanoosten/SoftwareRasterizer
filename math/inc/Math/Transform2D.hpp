#pragma once

#include "AABB.hpp"

#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>

namespace Math
{
class Transform2D
{
public:
    explicit Transform2D( const glm::vec2& position = glm::vec2 { 0 }, const glm::vec2& scale = glm::vec2 { 1 }, float rotation = 0.0f );

    /// <summary>
    /// Set the transform position.
    /// </summary>
    /// <param name="pos">The new position.</param>
    void setPosition( const glm::vec2& pos ) noexcept
    {
        m_Position       = pos;
        m_TransformDirty = true;
    }

    /// <summary>
    /// Get the current position of the transform.
    /// </summary>
    /// <returns>The current position of the transform.</returns>
    const glm::vec2& getPosition() const noexcept
    {
        return m_Position;
    }

    /// <summary>
    /// Set the anchor position for the transforms.
    /// </summary>
    /// <param name="anchor">The new anchor point.</param>
    void setAnchor( const glm::vec2& anchor ) noexcept
    {
        m_Anchor         = anchor;
        m_TransformDirty = true;
    }

    /// <summary>
    /// Get the anchor position of the transform.
    /// </summary>
    /// <returns>The anchor position of the transform.</returns>
    const glm::vec2& getAnchor() const noexcept
    {
        return m_Anchor;
    }

    void setRotation( float rot ) noexcept
    {
        m_Rotation       = rot;
        m_TransformDirty = true;
    }

    float getRotation() const noexcept
    {
        return m_Rotation;
    }

    void setScale( const glm::vec2& scale ) noexcept
    {
        m_Scale = scale;
        m_TransformDirty = true;
    }

    const glm::vec2& getScale() const noexcept
    {
        return m_Scale;
    }

    /// <summary>
    /// Add to the current translation of the transform.
    /// </summary>
    /// <param name="translation">The translation to apply to the position of the object.</param>
    void translate( const glm::vec2& translation ) noexcept
    {
        m_Position += translation;
        m_TransformDirty = true;
    }

    /// <summary>
    /// Apply a scale to the object.
    /// The current scale of the object is multiplied by this scale factor.
    /// </summary>
    /// <param name="factor">The scale factor to apply to the current scale of the object.</param>
    void scale( const glm::vec2& factor ) noexcept
    {
        m_Scale *= factor;
        m_TransformDirty = true;
    }

    void rotate( float rotation ) noexcept
    {
        m_Rotation += rotation;
        m_TransformDirty = true;
    }

    /// <summary>
    /// Get the 3x3 transform matrix.
    /// </summary>
    /// <returns></returns>
    const glm::mat3& getTransform() const noexcept;

private:
    // The anchor point determines the origin of the applied transformations.
    glm::vec2 m_Anchor { 0 };
    // The position of the object in 2D space.
    glm::vec2 m_Position { 0 };
    // The scale of the object in the local X and Y axis.
    glm::vec2 m_Scale { 1 };
    // The rotation angle (in radians) around the Z axis.
    float m_Rotation { 0.0f };

    // The 2D transformation matrix that combines the position, scale, and rotation in one.
    mutable glm::mat3 m_Transform { 1 };
    mutable bool      m_TransformDirty = true;
};

/// <summary>
/// Transform a 2D AABB by a Transform2D.
/// </summary>
/// <param name="aabb">The AABB to transform.</param>
/// <param name="transform">The transformation to apply.</param>
/// <returns>The transformed AABB.</returns>
inline AABB operator*( const Transform2D& transform, const AABB& aabb )
{
    auto& mat = transform.getTransform();
    const auto min = mat * glm::vec3 { aabb.min.x, aabb.min.y, 1 };
    const auto max = mat * glm::vec3 { aabb.max.x, aabb.max.y, 1 };

    return { glm::vec3 { min.x, min.y, aabb.min.z }, glm::vec3 { max.x, max.y, aabb.max.z } };
}

/// <summary>
/// Transform a 2D AABB by a Transform2D.
/// </summary>
/// <param name="aabb">The AABB to transform.</param>
/// <param name="transform">The transformation to apply.</param>
/// <returns>The transformed AABB.</returns>
inline AABB& operator*=( AABB& aabb, const Transform2D& transform )
{
    auto& mat = transform.getTransform();
    const auto min = mat * glm::vec3 { aabb.min.x, aabb.min.y, 1 };
    const auto max = mat * glm::vec3 { aabb.max.x, aabb.max.y, 1 };

    aabb.min = glm::vec3 { min.x, min.y, aabb.min.z };
    aabb.max = glm::vec3 { max.x, max.y, aabb.max.z };

    return aabb;
}

}  // namespace Math
