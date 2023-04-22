#pragma once

#include "Config.hpp"

#if !defined( USE_SSE )
    #define USE_SSE 0
#endif
#if !defined( USE_VCL )
    #define USE_VCL 0
#endif

#if USE_SSE
    #include <emmintrin.h>
    #include <immintrin.h>
    #include <smmintrin.h>
    #define SR_INLINE inline
#elif USE_VCL
    #include <vcl/vectorclass.h>
    #define SR_INLINE inline
#else
    #define SR_INLINE constexpr
#endif

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

    SR_INLINE Color  operator+( const Color& rhs ) const noexcept;
    SR_INLINE Color& operator+=( const Color& rhs ) noexcept;
    SR_INLINE Color  operator-( const Color& rhs ) const noexcept;
    SR_INLINE Color& operator-=( const Color& rhs ) noexcept;
    SR_INLINE Color  operator*( const Color& rhs ) const noexcept;
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
    /// <param name="color"></param>
    /// <returns></returns>
    static constexpr Color fromHex( uint32_t color ) noexcept;

    union
    {
        uint32_t argb;
        struct
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

constexpr Color::Color() noexcept
: b { 0 }
, g { 0 }
, r { 0 }
, a { 255 }
{}

constexpr Color::Color( uint32_t argb ) noexcept
: argb { argb }
{}

constexpr Color::Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) noexcept
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

SR_INLINE Color Color::operator+( const Color& _rhs ) const noexcept
{
#if USE_SSE
    static const __m128i _255 = _mm_set1_epi32( 255 );

    const __m128i lhs = _mm_set_epi32( a, r, g, b );
    const __m128i rhs = _mm_set_epi32( _rhs.a, _rhs.r, _rhs.g, _rhs.b );

    __m128i result = _mm_add_epi32( lhs, rhs );
    result         = _mm_min_epi32( result, _255 );

    const uint8_t blue  = static_cast<uint8_t>( _mm_extract_epi32( result, 0 ) );
    const uint8_t green = static_cast<uint8_t>( _mm_extract_epi32( result, 1 ) );
    const uint8_t red   = static_cast<uint8_t>( _mm_extract_epi32( result, 2 ) );
    const uint8_t alpha = static_cast<uint8_t>( _mm_extract_epi32( result, 3 ) );

    return { red, green, blue, alpha };
#elif USE_VCL != 0
    const Vec4i        lhs { b, g, r, a };
    const Vec4i        rhs { _rhs.b, _rhs.g, _rhs.r, _rhs.a };
    static const Vec4i _255 { 255 };

    Vec4i result = min( lhs + rhs, _255 );

    const uint8_t blue  = static_cast<uint8_t>( result[0] );
    const uint8_t green = static_cast<uint8_t>( result[1] );
    const uint8_t red   = static_cast<uint8_t>( result[2] );
    const uint8_t alpha = static_cast<uint8_t>( result[3] );

    return { red, green, blue, alpha };
#else
    return {
        static_cast<uint8_t>( std::min<uint32_t>( r + _rhs.r, 255u ) ),
        static_cast<uint8_t>( std::min<uint32_t>( g + _rhs.g, 255u ) ),
        static_cast<uint8_t>( std::min<uint32_t>( b + _rhs.b, 255u ) ),
        static_cast<uint8_t>( std::min<uint32_t>( a + _rhs.a, 255u ) ),
    };
#endif
}

SR_INLINE Color& Color::operator+=( const Color& _rhs ) noexcept
{
#if USE_SSE
    static const __m128i _255 = _mm_set1_epi32( 255 );

    const __m128i lhs = _mm_set_epi32( a, r, g, b );
    const __m128i rhs = _mm_set_epi32( _rhs.a, _rhs.r, _rhs.g, _rhs.b );

    __m128i result = _mm_add_epi32( lhs, rhs );
    result         = _mm_min_epi32( result, _255 );

    b = static_cast<uint8_t>( _mm_extract_epi32( result, 0 ) );
    g = static_cast<uint8_t>( _mm_extract_epi32( result, 1 ) );
    r = static_cast<uint8_t>( _mm_extract_epi32( result, 2 ) );
    a = static_cast<uint8_t>( _mm_extract_epi32( result, 3 ) );
#elif USE_VCL
    static const Vec4i _255 { 255 };

    const Vec4i lhs { b, g, r, a };
    const Vec4i rhs { _rhs.b, _rhs.g, _rhs.r, _rhs.a };

    Vec4i result = min( lhs + rhs, _255 );

    b = static_cast<uint8_t>( result[0] );
    g = static_cast<uint8_t>( result[1] );
    r = static_cast<uint8_t>( result[2] );
    a = static_cast<uint8_t>( result[3] );
#else
    b = static_cast<uint8_t>( std::min<uint32_t>( b + _rhs.b, 255u ) );
    g = static_cast<uint8_t>( std::min<uint32_t>( g + _rhs.g, 255u ) );
    r = static_cast<uint8_t>( std::min<uint32_t>( r + _rhs.r, 255u ) );
    a = static_cast<uint8_t>( std::min<uint32_t>( a + _rhs.a, 255u ) );
#endif

    return *this;
}

SR_INLINE Color Color::operator-( const Color& _rhs ) const noexcept
{
#if USE_SSE
    static const __m128i _0 = _mm_set1_epi32( 0 );

    const __m128i lhs = _mm_set_epi32( a, r, g, b );
    const __m128i rhs = _mm_set_epi32( _rhs.a, _rhs.r, _rhs.g, _rhs.b );

    __m128i result = _mm_sub_epi32( lhs, rhs );
    result         = _mm_max_epi32( result, _0 );

    const uint8_t blue  = static_cast<uint8_t>( _mm_extract_epi32( result, 0 ) );
    const uint8_t green = static_cast<uint8_t>( _mm_extract_epi32( result, 1 ) );
    const uint8_t red   = static_cast<uint8_t>( _mm_extract_epi32( result, 2 ) );
    const uint8_t alpha = static_cast<uint8_t>( _mm_extract_epi32( result, 3 ) );

    return { red, green, blue, alpha };
#elif USE_VCL
    static const Vec4i _0 { 0 };

    const Vec4i lhs { b, g, r, a };
    const Vec4i rhs { _rhs.b, _rhs.g, _rhs.r, _rhs.a };

    Vec4i result = max( lhs - rhs, _0 );

    const uint8_t blue  = static_cast<uint8_t>( result[0] );
    const uint8_t green = static_cast<uint8_t>( result[1] );
    const uint8_t red   = static_cast<uint8_t>( result[2] );
    const uint8_t alpha = static_cast<uint8_t>( result[3] );

    return { red, green, blue, alpha };
#else
    return {
        static_cast<uint8_t>( std::max<int32_t>( r - _rhs.r, 0 ) ),
        static_cast<uint8_t>( std::max<int32_t>( g - _rhs.g, 0 ) ),
        static_cast<uint8_t>( std::max<int32_t>( b - _rhs.b, 0 ) ),
        static_cast<uint8_t>( std::max<int32_t>( a - _rhs.a, 0 ) ),
    };
#endif
}

SR_INLINE Color& Color::operator-=( const Color& _rhs ) noexcept
{
#if USE_SSE
    static const __m128i _0 = _mm_set1_epi32( 0 );

    const __m128i lhs = _mm_set_epi32( a, r, g, b );
    const __m128i rhs = _mm_set_epi32( _rhs.a, _rhs.r, _rhs.g, _rhs.b );

    __m128i result = _mm_sub_epi32( lhs, rhs );
    result         = _mm_max_epi32( result, _0 );

    b = static_cast<uint8_t>( _mm_extract_epi32( result, 0 ) );
    g = static_cast<uint8_t>( _mm_extract_epi32( result, 1 ) );
    r = static_cast<uint8_t>( _mm_extract_epi32( result, 2 ) );
    a = static_cast<uint8_t>( _mm_extract_epi32( result, 3 ) );
#elif USE_VCL
    static const Vec4i _0 { 0 };

    const Vec4i lhs { b, g, r, a };
    const Vec4i rhs { _rhs.b, _rhs.g, _rhs.r, _rhs.a };

    Vec4i result = max( lhs - rhs, _0 );

    b = static_cast<uint8_t>( result[0] );
    g = static_cast<uint8_t>( result[1] );
    r = static_cast<uint8_t>( result[2] );
    a = static_cast<uint8_t>( result[3] );
#else
    b          = static_cast<uint8_t>( std::max<int32_t>( b - _rhs.b, 0 ) );
    g          = static_cast<uint8_t>( std::max<int32_t>( g - _rhs.g, 0 ) );
    r          = static_cast<uint8_t>( std::max<int32_t>( r - _rhs.r, 0 ) );
    a          = static_cast<uint8_t>( std::max<int32_t>( a - _rhs.a, 0 ) );
#endif
    return *this;
}

SR_INLINE Color Color::operator*( const Color& _rhs ) const noexcept
{
#if USE_SSE
    static const __m128i _255 = _mm_set1_epi32( 255 );

    const __m128i lhs = _mm_set_epi32( a, r, g, b );
    const __m128i rhs = _mm_set_epi32( _rhs.a, _rhs.r, _rhs.g, _rhs.b );

    __m128i result = _mm_mullo_epi32( lhs, rhs );
    result         = _mm_div_epi32( result, _255 );

    const uint8_t blue  = static_cast<uint8_t>( _mm_extract_epi32( result, 0 ) );
    const uint8_t green = static_cast<uint8_t>( _mm_extract_epi32( result, 1 ) );
    const uint8_t red   = static_cast<uint8_t>( _mm_extract_epi32( result, 2 ) );
    const uint8_t alpha = static_cast<uint8_t>( _mm_extract_epi32( result, 3 ) );

    return { red, green, blue, alpha };
#elif USE_VCL
    const Vec4i lhs { b, g, r, a };
    const Vec4i rhs { _rhs.b, _rhs.g, _rhs.r, _rhs.a };

    Vec4i result = ( lhs * rhs ) >> 8;

    const uint8_t blue = static_cast<uint8_t>( result[0] );
    const uint8_t green = static_cast<uint8_t>( result[1] );
    const uint8_t red = static_cast<uint8_t>( result[2] );
    const uint8_t alpha = static_cast<uint8_t>( result[3] );

    return { red, green, blue, alpha };
#else
    auto blue  = static_cast<uint8_t>( b * _rhs.b / 255 );
    auto green = static_cast<uint8_t>( g * _rhs.g / 255 );
    auto red   = static_cast<uint8_t>( r * _rhs.r / 255 );
    auto alpha = static_cast<uint8_t>( a * _rhs.a / 255 );

    return { red, green, blue, alpha };
#endif
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
    auto blue  = static_cast<uint8_t>( std::clamp( static_cast<float>( b ) * _rhs, 0.0f, 255.0f ) );
    auto green = static_cast<uint8_t>( std::clamp( static_cast<float>( g ) * _rhs, 0.0f, 255.0f ) );
    auto red   = static_cast<uint8_t>( std::clamp( static_cast<float>( r ) * _rhs, 0.0f, 255.0f ) );
    auto alpha = static_cast<uint8_t>( std::clamp( static_cast<float>( a ) * _rhs, 0.0f, 255.0f ) );

    return { red, green, blue, alpha };
}

inline Color operator*( float lhs, const Color& rhs )
{
    return rhs * lhs;
}

constexpr Color& Color::operator*=( float _rhs ) noexcept
{
    b = static_cast<uint8_t>( std::clamp( static_cast<float>( b ) * _rhs, 0.0f, 255.0f ) );
    g = static_cast<uint8_t>( std::clamp( static_cast<float>( g ) * _rhs, 0.0f, 255.0f ) );
    r = static_cast<uint8_t>( std::clamp( static_cast<float>( r ) * _rhs, 0.0f, 255.0f ) );
    a = static_cast<uint8_t>( std::clamp( static_cast<float>( a ) * _rhs, 0.0f, 255.0f ) );

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