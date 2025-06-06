#pragma once

#include "Button.hpp"
#include "State.hpp"

class Game;

class TitleState : public State
{
public:
    TitleState( Game& game );

    void update( float deltaTime ) override;
    void draw( Graphics::Image& image ) override;
    void processEvent( const Graphics::Event& event ) override;

private:
    void onResized( const Graphics::ResizeEventArgs& args );
    void onMouseMoved( Graphics::MouseMovedEventArgs& args );

    Game& game;

    int screenWidth;
    int screenHeight;

    Graphics::Sprite arkanoidSprite;
    Graphics::Sprite taitoSprite;

    // The game rectangle in the Window's coordinate frame.
    // Used for translating mouse coordinates.
    Math::RectI gameRect { 0 };
    // Translated mouse position.
    glm::ivec2 mousePos { 0 };
};
