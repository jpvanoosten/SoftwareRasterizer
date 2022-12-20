#pragma once

#include "Background.hpp"
#include "Button.hpp"


#include <Image.hpp>
#include <Timer.hpp>
#include <Font.hpp>
#include <Events.hpp>
#include <Math/Rect.hpp>

#include <LDtkLoader/Project.hpp>

#include <cstdint>

class Game
{
public:
    Game( uint32_t screenWidth, uint32_t screenHeight );

    Game( const Game& ) = delete;
    Game( Game&& )      = delete;
    Game& operator=( const Game& ) = delete;
    Game& operator=( Game&& )      = delete;

    void Update();

    const sr::Image& getImage() const noexcept
    {
        return image;
    }

    void processEvent( const sr::Event& event );

    void onKeyPressed( sr::KeyEventArgs& args );
    void onKeyReleased(sr::KeyEventArgs& args );
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

    // Fonts.
    sr::Font arial20;
    sr::Font arial24;

    // Backgrounds
    std::vector<Background> backgrounds;

    // Buttons
    Button previousButton;
    Button nextButton;
    Button restartButton;

    

};

