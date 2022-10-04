#pragma once

#include "Config.hpp"
#include "Color.hpp"

#include <memory>
#include <cassert>

namespace sr
{
    struct SR_API Image
    {
        Image(uint32_t width, uint32_t height);
        ~Image() = default;
        
        void clear(const Color& color);

        const Color& operator()(uint32_t x, uint32_t y) const
        {
            assert(x < m_width);
            assert(y < m_height);

            return m_data[y * m_width + x];
        }

        Color& operator()(uint32_t x, uint32_t y)
        {
            assert(x < m_width);
            assert(y < m_height);

            return m_data[y * m_width + x];
        }

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
}