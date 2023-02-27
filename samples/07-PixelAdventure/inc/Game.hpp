#pragma once

#include "Background.hpp"
#include "Button.hpp"
#include "Level.hpp"
#include "Transition.hpp"

#include <Graphics/Events.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/Image.hpp>
#include <Graphics/Timer.hpp>

#include <Math/Rect.hpp>

#include <LDtkLoader/Project.hpp>

#include <cstdint>

class Game final
{
public:
    Game( uint32_t screenWidth, uint32_t screenHeight );
    ~Game() = default;

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

    void loadLevel( size_t levelId, size_t characterId );

protected:
    enum class TransitionState
    {
        None,
        In,
        Out,
    };

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

    // Level transition effect.
    Transition      transition;
    const float     transitionDuration = 0.3f;
    float           transitionTime     = transitionDuration;
    TransitionState transitionState    = TransitionState::None;

    // Buttons
    Button previousButton;
    Button nextButton;
    Button restartButton;

    // Levels
    Level  currentLevel;
    size_t currentLevelId = 0u;
    // Which level to play next.
    size_t nextLevelId = 0u;

    // Player character.
    size_t currentCharacterId = 0u;
};
