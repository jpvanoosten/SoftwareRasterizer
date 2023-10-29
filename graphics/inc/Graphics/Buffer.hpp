#pragma once

#include <aligned_unique_ptr.hpp>

#include <cassert>
#include <cstddef>

namespace Graphics
{
template<typename T, std::size_t Alignment = 64>
struct Buffer
{
    Buffer()  = default;
    ~Buffer() = default;

    Buffer( std::size_t width, std::size_t height );

    Buffer( const Buffer& copy );
    Buffer( Buffer&& move ) noexcept;

    Buffer& operator=( const Buffer& rhs );
    Buffer& operator=( Buffer&& rhs ) noexcept;

    const T& operator[]( std::size_t i ) const;
    T&       operator[]( std::size_t i );

    const T& operator()( std::size_t x, std::size_t y ) const;
    T&       operator()( std::size_t x, std::size_t y );

    const T* data() const;
    T*       data();

    void resize( std::size_t width, std::size_t height );

    std::size_t getWidth() const noexcept;
    std::size_t getHeight() const noexcept;

    /// <summary>
    /// Clear the buffer contents to a specific value.
    /// </summary>
    /// <param name="v">The value to clear the buffer to.</param>
    void clear( const T& v );

private:
    std::size_t m_width  = 0u;
    std::size_t m_height = 0u;

    aligned_unique_ptr<T[]> m_data;
};

template<typename T, std::size_t Alignment>
Buffer<T, Alignment>::Buffer( std::size_t width, std::size_t height )
{
    resize( width, height );
}

template<typename T, std::size_t Alignment>
Buffer<T, Alignment>::Buffer( const Buffer& copy )
{
    resize( copy.m_width, copy.m_height );
    std::memcpy( data(), copy.data(), m_width * m_height * sizeof( T ) );
}

template<typename T, std::size_t Alignment>
Buffer<T, Alignment>::Buffer( Buffer&& move ) noexcept
: m_width { move.m_width }
, m_height { move.m_height }
, m_data { std::move( move.m_data ) }
{
    move.m_width  = 0u;
    move.m_height = 0u;
}

template<typename T, std::size_t Alignment>
Buffer<T, Alignment>& Buffer<T, Alignment>::operator=( const Buffer& rhs )
{
    resize( rhs.m_width, rhs.m_height );
    std::memcpy( data(), rhs.data(), m_width * m_height * sizeof( T ) );

    return *this;
}

template<typename T, std::size_t Alignment>
Buffer<T, Alignment>& Buffer<T, Alignment>::operator=( Buffer&& rhs ) noexcept
{
    m_width  = rhs.m_width;
    m_height = rhs.m_height;
    m_data   = std::move( rhs.m_data );

    rhs.m_width  = 0;
    rhs.m_height = 0;

    return *this;
}

template<typename T, std::size_t Alignment>
const T& Buffer<T, Alignment>::operator[]( std::size_t i ) const
{
    assert( i < m_width * m_height );
    return m_data[i];
}

template<typename T, std::size_t Alignment>
T& Buffer<T, Alignment>::operator[]( std::size_t i )
{
    assert( i < m_width * m_height );
    return m_data[i];
}

template<typename T, std::size_t Alignment>
const T& Buffer<T, Alignment>::operator()( std::size_t x, std::size_t y ) const
{
    assert( x < m_width );
    assert( y < m_height );

    return m_data[y * m_width + x];
}

template<typename T, std::size_t Alignment>
T& Buffer<T, Alignment>::operator()( std::size_t x, std::size_t y )
{
    assert( x < m_width );
    assert( y < m_height );

    return m_data[y * m_width + x];
}

template<typename T, std::size_t Alignment>
const T* Buffer<T, Alignment>::data() const
{
    return m_data.get();
}

template<typename T, std::size_t Alignment>
T* Buffer<T, Alignment>::data()
{
    return m_data.get();
}

template<typename T, std::size_t Alignment>
void Buffer<T, Alignment>::resize( std::size_t width, std::size_t height )
{
    if ( m_width != width || m_height != height )
    {
        m_data = make_aligned_unique<T[], Alignment>( width * height );

        m_width  = width;
        m_height = height;
    }
}

template<typename T, std::size_t Alignment>
std::size_t Buffer<T, Alignment>::getWidth() const noexcept
{
    return m_width;
}

template<typename T, std::size_t Alignment>
std::size_t Buffer<T, Alignment>::getHeight() const noexcept
{
    return m_height;
}

template<typename T, std::size_t Alignment>
void Buffer<T, Alignment>::clear( const T& v )
{
    T* p = data();

#pragma omp parallel for
    for ( int i = 0; i < static_cast<int>( m_width * m_height ); ++i )
        p[i] = v;
}

}  // namespace Graphics
