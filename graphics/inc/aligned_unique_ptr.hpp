#pragma once

#include <memory>

struct aligned_deleter
{
    void operator()( void* ptr ) const
    {
        // Note: this doesn't destruct array elements.
        // TODO: specialize aligned_deleter for array types?
        _aligned_free( ptr );
    }
};

namespace detail
{
template<typename>
constexpr bool is_unbounded_array_v = false;

template<typename T>
constexpr bool is_unbounded_array_v<T[]> = true;

template<typename>
constexpr bool is_bounded_array_v = false;

template<typename T, std::size_t N>
constexpr bool is_bounded_array_v<T[N]> = true;
}  // namespace detail

template<typename T>
using aligned_unique_ptr = std::unique_ptr<T, aligned_deleter>;

template<typename T, std::size_t Align, typename... Args>
std::enable_if_t<!std::is_array_v<T>, aligned_unique_ptr<T>>
    make_aligned_unique( Args&&... args )
{
    aligned_unique_ptr<T> ptr = aligned_unique_ptr<T>( static_cast<T*>( _aligned_malloc( sizeof( T ), Align ) ), aligned_deleter() );
    new ( ptr.get() ) T( std::forward<Args>( args )... );
    return ptr;
}

template<typename T, std::size_t Align>
std::enable_if_t<detail::is_unbounded_array_v<T> && std::is_default_constructible_v<std::remove_extent_t<T>>, aligned_unique_ptr<T>>
    make_aligned_unique( std::size_t n )
{
    using T2                  = std::remove_extent_t<T>;
    aligned_unique_ptr<T> ptr = aligned_unique_ptr<T>( static_cast<T2*>( _aligned_malloc( sizeof( T2 ) * n, Align ) ), aligned_deleter() );

    // Default construct the elements.
    T2* p = ptr.get();
    for ( std::size_t i = 0; i < n; ++i )
        new ( p++ ) T();

    return ptr;
}

template<typename T, typename... Args>
std::enable_if_t<detail::is_bounded_array_v<T>> make_aligned_unique( Args&&... ) = delete;