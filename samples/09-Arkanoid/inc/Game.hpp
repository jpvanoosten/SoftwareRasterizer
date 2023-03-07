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

    Graphics::Image& getImage() noexcept;

    int getCoins() const noexcept
    {
        return coins;
    }

private:
    void setState( GameState newState );

    GameState              currentState = GameState::None;
    std::unique_ptr<State> state;

    Graphics::Image image;
    // Fonts.
    Graphics::Font arcadeN;

    int numPlayers = 2;
    int coins      = 0;
    // Player 1 score.
    int score1 = 0;
    // Player 2 score.
    int score2 = 0;
    // Current high score.
    int highScore = 50000;
    
};