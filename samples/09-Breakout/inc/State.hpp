#pragma once

#include <Events.hpp>
#include <Image.hpp>

class State
{
public:
    virtual void update( float deltaTime )              = 0;
    virtual void draw( sr::Image& image )               = 0;
    virtual void processEvent( const sr::Event& event ) = 0;

    virtual ~State() = default;

protected:
    State()                          = default;
    State( const State& )            = default;
    State( State&& )                 = default;
    State& operator=( const State& ) = default;
    State& operator=( State&& )      = default;
};
