#pragma once

#include "Background.hpp"
#include "Button.hpp"
#include "Player.hpp"

#include <Events.hpp>
#include <Font.hpp>
#include <GamePadStateTracker.hpp>
#include <Image.hpp>
#include <Math/Rect.hpp>
#include <Timer.hpp>

#include <LDtkLoader/Project.hpp>

#include <cstdint>

class Game
{
public:
    Game( uint32_t screenWidth, uint32_t screenHeight );

    Game( const Game& )            = delete;
    Game( Game&& )                 = delete;
    Game& operator=( const Game& ) = delete;
    Game& operator=( Game&& )      = delete;

    void Update();

    const sr::Image& getImage() const noexcept
    {
        return image;
    }

    void processEvent( const sr::Event& event );

    void onMouseMoved( sr::MouseMovedEventArgs& args );
    void onResized( sr::ResizeEventArgs& args );

    // Button handlers.
    void onPreviousClicked();
    void onNextClicked();
    void onRestartClicked();

protected:
    ldtk::Project ldtkProject;

    sr::Image image;
    sr::Timer timer;
    // The game rectangle in the Window's coordinate frame.
    // Used for translating mouse coordinates.
    Math::RectI gameRect;

    glm::ivec2 mousePos;
    sr::GamePadStateTracker gamePadStateTracker;

    // Fonts.
    sr::Font arial20;
    sr::Font arial24;

    // Backgrounds
    std::vector<Background> backgrounds;

    // Buttons
    Button previousButton;
    Button nextButton;
    Button restartButton;

    // Player
    Player player;
};
