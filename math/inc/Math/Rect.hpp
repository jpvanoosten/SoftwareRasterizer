#pragma once

#include <cstdint>
#include <compare>

#include <glm/vec2.hpp>

namespace Math
{

template<typename T>
struct Rect
{
    Rect( T left = T( 0 ), T top = T( 0 ), T width = T( 0 ), T height = T( 0 ) )
    : left { left }
    , top { top }
    , width { width }
    , height { height }
    {}

    auto operator<=>( const Rect& ) const = default;

    T right() const noexcept
    {
        return left + width;
    }

    T bottom() const noexcept
    {
        return top + height;
    }

    glm::vec<2, T> center() const noexcept
    {
        return { left + width / T(2), top + height / T(2) };
    }

    glm::vec<2, T> topLeft() const noexcept
    {
        return { left, top };
    }

    glm::vec<2, T> topRight() const noexcept
    {
        return { right(), top };
    }

    glm::vec<2, T> bottomLeft() const noexcept
    {
        return { left, bottom() };
    }

    glm::vec<2, T> bottomRight() const noexcept
    {
        return { right(), bottom() };
    }

    /// <summary>
    /// The left (x) coordinate of the rectangle.
    /// </summary>
    T left;

    /// <summary>
    /// The top (y) coordinate of the rectangle.
    /// </summary>
    T top;

    /// <summary>
    /// The width of the rectangle.
    /// </summary>
    T width;

    /// <summary>
    /// The height of the rectangle.
    /// </summary>
    T height;
};

using RectI  = Rect<int32_t>;
using RectUI = Rect<uint32_t>;
using RectF  = Rect<float>;
using RectD  = Rect<double>;

}  // namespace Math
