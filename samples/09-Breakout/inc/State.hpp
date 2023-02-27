#pragma once

#include <Graphics/Events.hpp>
#include <Graphics/Image.hpp>

class State
{
public:
    virtual void update( float deltaTime ) {}
    virtual void draw( Graphics::Image& image ) {}
    virtual void processEvent( const Graphics::Event& event ) {}

    virtual ~State() = default;

protected:
    State()                          = default;
    State( const State& )            = default;
    State( State&& )                 = default;
    State& operator=( const State& ) = default;
    State& operator=( State&& )      = default;
};
