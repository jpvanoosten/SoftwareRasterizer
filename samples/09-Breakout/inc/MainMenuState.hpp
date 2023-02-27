#pragma once

#include "State.hpp"

class MainMenuState : public State
{
public:
    MainMenuState(int screenWidth, int screenHeight);

    void update( float deltaTime ) override;
    void draw( Graphics::Image& image ) override;
    void processEvent( const Graphics::Event& event ) override;

private:
    int screenWidth;
    int screenHeight;
};