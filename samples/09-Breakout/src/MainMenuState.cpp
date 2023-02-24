#include <MainMenuState.hpp>

MainMenuState::MainMenuState( int screenWidth, int screenHeight )
: screenWidth { screenWidth }
, screenHeight { screenHeight }
{}

void MainMenuState::update( float deltaTime ) {}
void MainMenuState::draw( sr::Image& image ) {}
void MainMenuState::processEvent( const sr::Event& event ) {}
