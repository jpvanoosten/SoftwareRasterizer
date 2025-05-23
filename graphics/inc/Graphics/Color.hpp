#pragma once

#include "Config.hpp"

#include <Math/Math.hpp>

#include <glm/vec3.hpp>

#include <algorithm>
#include <cassert>
#include <compare>
#include <cstdint>

namespace Graphics
{
struct SR_API alignas( 4 ) Color
{
    constexpr Color() noexcept;
    constexpr explicit Color( uint32_t argb ) noexcept;
    constexpr Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255u ) noexcept;
    constexpr ~Color() noexcept              = default;
    constexpr Color( const Color& ) noexcept = default;
    constexpr Color( Color&& ) noexcept      = default;

    constexpr Color& operator=( const Color& ) noexcept = default;
    constexpr Color& operator=( Color&& ) noexcept      = default;

    constexpr auto operator<=>( const Color& rhs ) const noexcept;
    constexpr bool operator==( const Color& rhs ) const noexcept;

    constexpr Color  operator+( const Color& rhs ) const noexcept;
    constexpr Color& operator+=( const Color& rhs ) noexcept;
    constexpr Color  operator-( const Color& rhs ) const noexcept;
    constexpr Color& operator-=( const Color& rhs ) noexcept;
    constexpr Color  operator*( const Color& rhs ) const noexcept;
    constexpr Color& operator*=( const Color& rhs ) noexcept;
    constexpr Color  operator*( float rhs ) const noexcept;
    constexpr Color& operator*=( float rhs ) noexcept;
    constexpr Color  operator/( float rhs ) const noexcept;
    constexpr Color& operator/=( float rhs ) noexcept;

    /// <summary>
    /// Return this color with a specific alpha value.
    /// </summary>
    /// <param name="alpha">The alpha value.</param>
    /// <returns>This color with a given alpha value.</returns>
    constexpr Color withAlpha( float alpha ) const noexcept;

    /// <summary>
    /// Return this color with a specific alpha value.
    /// </summary>
    /// <param name="alpha">The alpha value.</param>
    /// <returns>This color with a given alpha value.</returns>
    constexpr Color withAlpha( uint8_t alpha ) const noexcept;

    /// <summary>
    /// Construct a color using floating-point values in the range [0 .. 1].
    /// </summary>
    /// <param name="r">The red color component.</param>
    /// <param name="g">The green color component.</param>
    /// <param name="b">The blue color component.</param>
    /// <param name="a">)(optional) The alpha component. Default: 1.0f</param>
    /// <returns>The color.</returns>
    static constexpr Color fromFloats( float r, float g, float b, float a = 1.0 ) noexcept;

    /// <summary>
    /// Construct a color from a 32-bit unsigned integer (usually expressed as a hexadecimal value).
    /// The color is expressed as:
    /// 0xAARRGGBB
    /// The alpha (AA) values can be omitted from the hex value, but the default alpha value will be 0.
    /// So (transparent) Red is:
    /// 0xFF0000
    /// And (transparent) Green is:
    /// 0xFF00
    /// And (transparent) Blue is:
    /// 0xFF
    /// </summary>
    /// <param name="color">The color as a 32-bit integer.</param>
    /// <returns></returns>
    static constexpr Color fromHex( uint32_t color ) noexcept;

    /// <summary>
    /// Construct a color from Hue, Saturation, and Brightness values.
    /// See: https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
    /// </summary>
    /// <param name="H"></param>
    /// <param name="S"></param>
    /// <param name="V"></param>
    /// <returns></returns>
    static inline Color fromHSV( float H, float S, float V ) noexcept;

    union
    {
        uint32_t argb;
        // Color components.
        struct  // NOLINT(clang-diagnostic-nested-anon-types)
        {
            uint8_t b;
            uint8_t g;
            uint8_t r;
            uint8_t a;
        };
    };

    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Magenta;
    static const Color Yellow;
    static const Color Cyan;
};

static_assert( sizeof( Color ) == 4 );

constexpr Color::Color() noexcept  // NOLINT(cppcoreguidelines-pro-type-member-init)
: b { 0 }
, g { 0 }
, r { 0 }
, a { 255 }
{}

constexpr Color::Color( uint32_t argb ) noexcept  // NOLINT(cppcoreguidelines-pro-type-member-init)
: argb { argb }
{}

constexpr Color::Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) noexcept  // NOLINT(cppcoreguidelines-pro-type-member-init)
: b { b }
, g { g }
, r { r }
, a { a }
{}

constexpr bool Color::operator==( const Color& rhs ) const noexcept
{
    return b == rhs.b && g == rhs.g && r == rhs.r && a == rhs.a;
}

constexpr auto Color::operator<=>( const Color& rhs ) const noexcept
{
    if ( const auto cmp = b <=> rhs.b; cmp != 0 )
        return cmp;
    if ( const auto cmp = g <=> rhs.g; cmp != 0 )
        return cmp;
    if ( const auto cmp = r <=> rhs.r; cmp != 0 )
        return cmp;

    return a <=> rhs.a;
}

constexpr Color Color::operator+( const Color& _rhs ) const noexcept
{
    return {
        static_cast<uint8_t>( Math::min<uint32_t>( r + _rhs.r, 255u ) ),
        static_cast<uint8_t>( Math::min<uint32_t>( g + _rhs.g, 255u ) ),
        static_cast<uint8_t>( Math::min<uint32_t>( b + _rhs.b, 255u ) ),
        static_cast<uint8_t>( Math::min<uint32_t>( a + _rhs.a, 255u ) ),
    };
}

constexpr Color& Color::operator+=( const Color& _rhs ) noexcept
{
    b = static_cast<uint8_t>( Math::min<uint32_t>( b + _rhs.b, 255u ) );
    g = static_cast<uint8_t>( Math::min<uint32_t>( g + _rhs.g, 255u ) );
    r = static_cast<uint8_t>( Math::min<uint32_t>( r + _rhs.r, 255u ) );
    a = static_cast<uint8_t>( Math::min<uint32_t>( a + _rhs.a, 255u ) );

    return *this;
}

constexpr Color Color::operator-( const Color& _rhs ) const noexcept
{
    return {
        static_cast<uint8_t>( Math::max<int32_t>( r - _rhs.r, 0 ) ),
        static_cast<uint8_t>( Math::max<int32_t>( g - _rhs.g, 0 ) ),
        static_cast<uint8_t>( Math::max<int32_t>( b - _rhs.b, 0 ) ),
        static_cast<uint8_t>( Math::max<int32_t>( a - _rhs.a, 0 ) ),
    };
}

constexpr Color& Color::operator-=( const Color& _rhs ) noexcept
{
    b = static_cast<uint8_t>( Math::max<int32_t>( b - _rhs.b, 0 ) );
    g = static_cast<uint8_t>( Math::max<int32_t>( g - _rhs.g, 0 ) );
    r = static_cast<uint8_t>( Math::max<int32_t>( r - _rhs.r, 0 ) );
    a = static_cast<uint8_t>( Math::max<int32_t>( a - _rhs.a, 0 ) );

    return *this;
}

constexpr Color Color::operator*( const Color& _rhs ) const noexcept
{
    auto blue  = static_cast<uint8_t>( b * _rhs.b / 255 );
    auto green = static_cast<uint8_t>( g * _rhs.g / 255 );
    auto red   = static_cast<uint8_t>( r * _rhs.r / 255 );
    auto alpha = static_cast<uint8_t>( a * _rhs.a / 255 );

    return { red, green, blue, alpha };
}

constexpr Color& Color::operator*=( const Color& _rhs ) noexcept
{
    b = static_cast<uint8_t>( b * _rhs.b / 255 );
    g = static_cast<uint8_t>( g * _rhs.g / 255 );
    r = static_cast<uint8_t>( r * _rhs.r / 255 );
    a = static_cast<uint8_t>( a * _rhs.a / 255 );

    return *this;
}

constexpr Color Color::operator*( float _rhs ) const noexcept
{
    auto blue  = static_cast<uint8_t>( Math::clamp( static_cast<float>( b ) * _rhs, 0.0f, 255.0f ) );
    auto green = static_cast<uint8_t>( Math::clamp( static_cast<float>( g ) * _rhs, 0.0f, 255.0f ) );
    auto red   = static_cast<uint8_t>( Math::clamp( static_cast<float>( r ) * _rhs, 0.0f, 255.0f ) );
    auto alpha = static_cast<uint8_t>( Math::clamp( static_cast<float>( a ) * _rhs, 0.0f, 255.0f ) );

    return { red, green, blue, alpha };
}

constexpr Color operator*( float lhs, const Color& rhs )
{
    return rhs * lhs;
}

constexpr Color& Color::operator*=( float _rhs ) noexcept
{
    b = static_cast<uint8_t>( Math::clamp( static_cast<float>( b ) * _rhs, 0.0f, 255.0f ) );
    g = static_cast<uint8_t>( Math::clamp( static_cast<float>( g ) * _rhs, 0.0f, 255.0f ) );
    r = static_cast<uint8_t>( Math::clamp( static_cast<float>( r ) * _rhs, 0.0f, 255.0f ) );
    a = static_cast<uint8_t>( Math::clamp( static_cast<float>( a ) * _rhs, 0.0f, 255.0f ) );

    return *this;
}

constexpr Color Color::operator/( float _rhs ) const noexcept
{
    assert( _rhs != 0.0f );

    _rhs = 1.0f / _rhs;

    return operator*( _rhs );
}

constexpr Color& Color::operator/=( float _rhs ) noexcept
{
    assert( _rhs != 0.0f );

    _rhs = 1.0f / _rhs;

    return operator*=( _rhs );
}

constexpr Color Color::withAlpha( float alpha ) const noexcept
{
    return withAlpha( static_cast<uint8_t>( alpha * 255 ) );
}

constexpr Color Color::withAlpha( uint8_t alpha ) const noexcept
{
    return { r, g, b, alpha };
}

constexpr Color Color::fromFloats( float _r, float _g, float _b, float _a ) noexcept
{
    const uint8_t b = static_cast<uint8_t>( _b * 255.0f );
    const uint8_t g = static_cast<uint8_t>( _g * 255.0f );
    const uint8_t r = static_cast<uint8_t>( _r * 255.0f );
    const uint8_t a = static_cast<uint8_t>( _a * 255.0f );

    return { r, g, b, a };
}

constexpr Color Color::fromHex( uint32_t color ) noexcept
{
    const uint8_t b = static_cast<uint8_t>( ( color & 0x000000FF ) >> 0 );
    const uint8_t g = static_cast<uint8_t>( ( color & 0x0000FF00 ) >> 8 );
    const uint8_t r = static_cast<uint8_t>( ( color & 0x00FF0000 ) >> 16 );
    const uint8_t a = static_cast<uint8_t>( ( color & 0xFF000000 ) >> 24 );

    return { r, g, b, a };
}

inline Color Color::fromHSV( float H, float S, float V ) noexcept
{
    float C  = V * S;
    float m  = V - C;
    float H2 = H / 60.0f;
    float X  = C * ( 1.0f - fabsf( fmodf( H2, 2.0f ) - 1.0f ) );

    glm::vec3 RGB;

    switch ( static_cast<int>( H2 ) )
    {
    case 0:
        RGB = { C, X, 0 };
        break;
    case 1:
        RGB = { X, C, 0 };
        break;
    case 2:
        RGB = { 0, C, X };
        break;
    case 3:
        RGB = { 0, X, C };
        break;
    case 4:
        RGB = { X, 0, C };
        break;
    case 5:
        RGB = { C, 0, X };
        break;
    }

    RGB = RGB + m;

    return fromFloats( RGB.r, RGB.g, RGB.b );
}

/// <summary>
/// Return the component-wise minimum of two colors.
/// </summary>
/// <param name="c1">The first color.</param>
/// <param name="c2">The second color.</param>
/// <returns>The component-wise minimum of the two colors.</returns>
constexpr Color min( const Color& c1, const Color& c2 )
{
    const auto b = std::min( c1.b, c2.b );
    const auto g = std::min( c1.g, c2.g );
    const auto r = std::min( c1.r, c2.r );
    const auto a = std::min( c1.a, c2.a );

    return { r, g, b, a };
}

/// <summary>
/// Return the component-wise maximum of two colors.
/// </summary>
/// <param name="c1">The first color.</param>
/// <param name="c2">The second color.</param>
/// <returns>The component-wise minimum of the two colors.</returns>
constexpr Color max( const Color& c1, const Color& c2 )
{
    const auto b = std::max( c1.b, c2.b );
    const auto g = std::max( c1.g, c2.g );
    const auto r = std::max( c1.r, c2.r );
    const auto a = std::max( c1.a, c2.a );

    return { r, g, b, a };
}

}  // namespace Graphics