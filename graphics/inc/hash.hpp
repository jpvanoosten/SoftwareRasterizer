#pragma once

#include <functional>  // std::hash

// This is stolen from boost.
template<std::size_t Bits>
struct hash_mix_impl;

template<>
struct hash_mix_impl<64>
{
    static std::uint64_t mix( std::uint64_t x )
    {
        constexpr std::uint64_t m = ( static_cast<std::uint64_t>( 0xe9846af ) << 32 ) + 0x9b1a615d;

        x ^= x >> 32;
        x *= m;
        x ^= x >> 32;
        x *= m;
        x ^= x >> 28;

        return x;
    }
};

template<>
struct hash_mix_impl<32>
{
    static std::uint32_t mix( std::uint32_t x )
    {
        constexpr std::uint32_t m1 = 0x21f0aaad;
        constexpr std::uint32_t m2 = 0x735a2d97;

        x ^= x >> 16;
        x *= m1;
        x ^= x >> 15;
        x *= m2;
        x ^= x >> 15;

        return x;
    }
};

inline std::size_t hash_mix( std::size_t v )
{
    return hash_mix_impl<sizeof( std::size_t ) * CHAR_BIT>::mix( v );
}

template<typename T>
void hash_combine( std::size_t& seed, const T& v )
{
    seed = hash_mix( seed + 0x9e3779b9 + std::hash<T>()( v ) );
}
