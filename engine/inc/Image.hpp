#pragma once

#include "Config.hpp"
#include "Color.hpp"

#include <memory>

namespace sr
{
    struct ENGINE_API Image
    {
        Image(uint32_t width, uint32_t height);
        ~Image() = default;
        
        void clear(const Color& color);

        inline void set(uint32_t x, uint32_t y, const Color& color);

        uint32_t getWidth() const noexcept
        {
            return m_width;
        }

        uint32_t getHeight() const noexcept
        {
            return m_height;
        }

        Color* data() noexcept
        {
            return m_data.get();
        }

        const Color* data() const noexcept
        {
            return m_data.get();
        }

    private:
        uint32_t m_width;
        uint32_t m_height;
        std::unique_ptr<Color[]> m_data;
    };

    inline void Image::set(uint32_t x, uint32_t y, const Color& color)
    {
        assert(x < m_width);
        assert(y < m_height);

        const uint32_t i = y * m_width + x;

        m_data[i] = color;
    }
}