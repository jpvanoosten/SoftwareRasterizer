#include <MainMenuState.hpp>

MainMenuState::MainMenuState( int screenWidth, int screenHeight )
: screenWidth { screenWidth }
, screenHeight { screenHeight }
{}

void MainMenuState::update( float deltaTime ) {}
void MainMenuState::draw( Graphics::Image& image ) {}
void MainMenuState::processEvent( const Graphics::Event& event ) {}
