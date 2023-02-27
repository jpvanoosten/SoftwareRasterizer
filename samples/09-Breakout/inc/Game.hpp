#pragma once

#include "State.hpp"

#include <Graphics/Events.hpp>
#include <Graphics/Font.hpp>

#include <memory>

class Game
{
public:
    enum class GameState
    {
        None,
        MainMenu,
        Playing,
        Pause,
        GameOver,
    };

    Game( uint32_t screenWidth, uint32_t screenHeight );

    // Delete copy and move constructors and assign operators
    Game( const Game& )            = delete;
    Game( Game&& )                 = delete;
    Game& operator=( const Game& ) = delete;
    Game& operator=( Game&& )      = delete;

    void update( float deltaTime );

    void processEvent( const Graphics::Event& event );

    const Graphics::Image& getImage() const;

private:
    void setState( GameState newState );

    GameState              currentState = GameState::None;
    std::unique_ptr<State> state;

    Graphics::Image image;
    // Fonts.
    Graphics::Font arial20;
    Graphics::Font arial24;
};