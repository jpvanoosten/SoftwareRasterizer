#pragma once

#include "Background.hpp"
#include "Button.hpp"
#include "Level.hpp"

#include <Events.hpp>
#include <Font.hpp>
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
    ldtk::Project project;

    sr::Image image;
    sr::Timer timer;

    // Maximum tick time for physics.
    const float physicsTick = 1.0f / 60.0f;

    // The game rectangle in the Window's coordinate frame.
    // Used for translating mouse coordinates.
    Math::RectI gameRect;

    // Translated mouse position.
    glm::ivec2 mousePos;

    // Fonts.
    sr::Font arial20;
    sr::Font arial24;

    // Backgrounds
    using BackgroundList = std::vector<Background>;
    BackgroundList           backgrounds;
    BackgroundList::iterator currentBackground;

    // Buttons
    Button previousButton;
    Button nextButton;
    Button restartButton;

    // Levels
    Level     currentLevel;
    size_t    currentLevelId = 0u;
};
