#pragma once

#include <Events.hpp>
#include <Image.hpp>

class GameState
{
public:
    virtual void update( float deltaTime )              = 0;
    virtual void draw( sr::Image& image )               = 0;
    virtual void processEvent( const sr::Event& event ) = 0;

    virtual ~GameState() = default;
protected:
    GameState();
    GameState( const GameState& )            = default;
    GameState( GameState&& )                 = default;
    GameState& operator=( const GameState& ) = default;
    GameState& operator=( GameState&& )      = default;
};
