#pragma once

#include <functional>

template<typename T>
class Curve
{
public:
    Curve() = default;

    explicit Curve( std::function<T( T )> func )
    : func { std::move( func ) }
    {}

    T operator()( const T& x ) const
    {
        currentValue = x;
        if ( func )
            return func( x );

        return T( 0 );
    }

    T operator()() const
    {
        if ( func )
            return func( currentValue );

        return T( 0 );
    }

    Curve& operator=( const T& x )
    {
        currentValue = x;

        return *this;
    }

    Curve& operator+=( const T& x )
    {
        currentValue += x;

        return *this;
    }

private:
    std::function<T( T )> func;
    mutable T             currentValue = T( 0 );
};
