#pragma once

#include "Button.hpp"
#include "State.hpp"

class MainMenuState : public State
{
public:
    MainMenuState( int screenWidth, int screenHeight );

    void update( float deltaTime ) override;
    void draw( Graphics::Image& image ) override;
    void processEvent( const Graphics::Event& event ) override;

private:
    void onResized( const Graphics::ResizeEventArgs& args );
    void onMouseMoved( Graphics::MouseMovedEventArgs& args );

    int screenWidth;
    int screenHeight;

    static const int buttonWidth  = 200;
    static const int buttonHeight = 50;

    std::shared_ptr<Graphics::Font> buttonFont;

    Button startButton;
    Button quitButton;

    // The game rectangle in the Window's coordinate frame.
    // Used for translating mouse coordinates.
    Math::RectI gameRect;
    // Translated mouse position.
    glm::ivec2 mousePos;
};