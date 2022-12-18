#pragma once

#include "Background.hpp"

#include <Image.hpp>
#include <Timer.hpp>
#include <Font.hpp>
#include <Events.hpp>
#include <Math/Rect.hpp>

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

    void onKeyPressed( const sr::KeyEventArgs& args );
    void onKeyReleased( const sr::KeyEventArgs& args );
    void onMouseMoved( const sr::MouseMovedEventArgs& args );
    void onResized( const sr::ResizeEventArgs& args );

protected:
    sr::Image image;
    sr::Timer timer;
    // The game rectangle in the Window's coordinate frame.
    // Used for translating mouse coordinates.
    Math::RectI gameRect;

    glm::ivec2 mousePos;

    // Fonts.
    sr::Font arial20;
    sr::Font arial24;

    Background background;
};

