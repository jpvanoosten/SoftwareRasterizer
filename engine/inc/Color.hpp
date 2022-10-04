#pragma once

#include "Config.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <compare>

namespace sr
{
    struct SR_API alignas(4) Color
    {
    public:
        constexpr Color() noexcept;
        constexpr explicit Color(uint32_t rgba) noexcept;
        constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255u) noexcept;
        constexpr ~Color() noexcept = default;
        constexpr Color(const Color&) noexcept = default;
        constexpr Color(Color&&) noexcept = default;

        constexpr Color& operator=(const Color&) noexcept = default;
        constexpr Color& operator=(Color&&) noexcept = default;

        constexpr auto operator<=>(const Color& rhs) const noexcept;
        constexpr bool operator==(const Color& rhs) const noexcept;

        constexpr Color operator+(const Color& rhs) const noexcept;
        constexpr Color& operator+=(const Color& rhs) noexcept;
        constexpr Color operator-(const Color& rhs) const noexcept;
        constexpr Color& operator-=(const Color& rhs) noexcept;
        constexpr Color operator*(const Color& rhs) const noexcept;
        constexpr Color& operator*=(const Color& rhs) noexcept;
        constexpr Color operator*(float rhs) const noexcept;
        constexpr Color& operator*=(float rhs) noexcept;
        constexpr Color operator/(float rhs) const noexcept;
        constexpr Color& operator/=(float rhs) noexcept;

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
        : a{ 255 }
        , r{ 0 }
        , g{ 0 }
        , b{ 0 }
    {}

    constexpr Color::Color(uint32_t argb) noexcept
        :argb{ argb }
    {}

    constexpr Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept
        : a{ a }
        , r{ r }
        , g{ g }
        , b{ b }
    {}

    constexpr bool Color::operator==(const Color& rhs) const noexcept
    {
        return a == rhs.a && r == rhs.r && g == rhs.g && b == rhs.b;
    }

    constexpr auto Color::operator<=>(const Color& rhs) const noexcept
    {
        if (const auto cmp = a <=> rhs.a; cmp != 0)
            return cmp;
        if (const auto cmp = r <=> rhs.r; cmp != 0)
            return cmp;
        if (const auto cmp = g <=> rhs.g; cmp != 0)
            return cmp;

        return b <=> rhs.b;
    }

    constexpr Color Color::operator+(const Color& rhs) const noexcept
    {
        return {
            static_cast<uint8_t>(std::min<uint32_t>(r + rhs.r, 255u)),
            static_cast<uint8_t>(std::min<uint32_t>(g + rhs.g, 255u)),
            static_cast<uint8_t>(std::min<uint32_t>(b + rhs.b, 255u)),
            static_cast<uint8_t>(std::min<uint32_t>(a + rhs.a, 255u)),
        };
    }

    constexpr Color& Color::operator+=(const Color& rhs) noexcept
    {
        r = static_cast<uint8_t>(std::min<uint32_t>(r + rhs.r, 255u));
        g = static_cast<uint8_t>(std::min<uint32_t>(g + rhs.g, 255u));
        b = static_cast<uint8_t>(std::min<uint32_t>(b + rhs.b, 255u));
        a = static_cast<uint8_t>(std::min<uint32_t>(a + rhs.a, 255u));

        return *this;
    }

    constexpr Color Color::operator-(const Color& rhs) const noexcept
    {
        return {
            static_cast<uint8_t>(std::max<int32_t>(r - rhs.r, 0)),
            static_cast<uint8_t>(std::max<int32_t>(g - rhs.g, 0)),
            static_cast<uint8_t>(std::max<int32_t>(b - rhs.b, 0)),
            static_cast<uint8_t>(std::max<int32_t>(a - rhs.a, 0)),
        };
    }

    constexpr Color& Color::operator-=(const Color& rhs) noexcept
    {
        r = static_cast<uint8_t>(std::max<int32_t>(r - rhs.r, 0));
        g = static_cast<uint8_t>(std::max<int32_t>(g - rhs.g, 0));
        b = static_cast<uint8_t>(std::max<int32_t>(b - rhs.b, 0));
        a = static_cast<uint8_t>(std::max<int32_t>(a - rhs.a, 0));

        return *this;
    }

    constexpr Color Color::operator*(const Color& rhs) const noexcept
    {
        auto alpha = static_cast<uint8_t>(a * rhs.a / 255);
        auto red = static_cast<uint8_t>(r * rhs.r / 255);
        auto green = static_cast<uint8_t>(g * rhs.g / 255);
        auto blue = static_cast<uint8_t>(b * rhs.b / 255);

        return { red, green, blue, alpha };
    }

    constexpr Color& Color::operator*=(const Color& rhs) noexcept
    {
        r = static_cast<uint8_t>(r * rhs.r / 255);
        g = static_cast<uint8_t>(g * rhs.g / 255);
        b = static_cast<uint8_t>(b * rhs.b / 255);
        a = static_cast<uint8_t>(a * rhs.a / 255);

        return *this;
    }

    constexpr Color Color::operator*(float rhs) const noexcept
    {
        auto red = static_cast<uint8_t>(std::clamp(static_cast<float>(r) * rhs, 0.0f, 255.0f));
        auto green = static_cast<uint8_t>(std::clamp(static_cast<float>(g) * rhs, 0.0f, 255.0f));
        auto blue = static_cast<uint8_t>(std::clamp(static_cast<float>(b) * rhs, 0.0f, 255.0f));
        auto alpha = static_cast<uint8_t>(std::clamp(static_cast<float>(a) * rhs, 0.0f, 255.0f));

        return { red, green, blue, alpha };
    }

    constexpr Color& Color::operator*=(float rhs) noexcept
    {
        r = static_cast<uint8_t>(std::clamp(static_cast<float>(r) * rhs, 0.0f, 255.0f));
        g = static_cast<uint8_t>(std::clamp(static_cast<float>(g) * rhs, 0.0f, 255.0f));
        b = static_cast<uint8_t>(std::clamp(static_cast<float>(b) * rhs, 0.0f, 255.0f));
        a = static_cast<uint8_t>(std::clamp(static_cast<float>(a) * rhs, 0.0f, 255.0f));

        return *this;
    }

    constexpr Color Color::operator/(float rhs) const noexcept
    {
        assert(rhs != 0.0f);

        rhs = 1.0f / rhs;

        return operator*(rhs);
    }

    constexpr Color& Color::operator/=(float rhs) noexcept
    {
        assert(rhs != 0.0f);

        rhs = 1.0f / rhs;

        return operator*=(rhs);
    }

}