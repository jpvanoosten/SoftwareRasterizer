#pragma once

#include "State.hpp"

class MainMenuState : public State
{
public:
    MainMenuState(int screenWidth, int screenHeight);

    void update( float deltaTime ) override;
    void draw( sr::Image& image ) override;
    void processEvent( const sr::Event& event ) override;

private:
    int screenWidth;
    int screenHeight;
};