#pragma once

#include <Graphics/Events.hpp>
#include <Graphics/Image.hpp>

class State
{
public:
    virtual void update( float deltaTime )              = 0;
    virtual void draw( Graphics::Image& image )               = 0;
    virtual void processEvent( const Graphics::Event& event ) = 0;

    virtual ~State() = default;

protected:
    State()                          = default;
    State( const State& )            = default;
    State( State&& )                 = default;
    State& operator=( const State& ) = default;
    State& operator=( State&& )      = default;
};
