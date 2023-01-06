#include <Game.hpp>

#include <Color.hpp>
#include <Input.hpp>

#include <string>

using namespace sr;
using namespace Math;

Game::Game( uint32_t screenWidth, uint32_t screenHeight )
: image { screenWidth, screenHeight }
, arial20 { "assets/fonts/arial.ttf", 20 }
, arial24 { "assets/fonts/arial.ttf", 24 }
{
    // Input actions map
    Input::mapAxis( "Horizontal", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        float leftX = 0.0f;

        for ( auto& gamePadState: gamePadStates )
        {
            const auto state = gamePadState.getLastState();

            leftX += state.thumbSticks.leftX;
        }

        const auto keyState = keyboardState.getLastState();

        const float a     = keyState.A ? 1.0f : 0.0f;
        const float d     = keyState.D ? 1.0f : 0.0f;
        const float left  = keyState.Left ? 1.0f : 0.0f;
        const float right = keyState.Right ? 1.0f : 0.0f;

        return std::clamp( leftX - a + d - left + right, -1.0f, 1.0f );
    } );

    Input::mapButtonDown( "Jump", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
        bool a = false;

        for ( auto& gamePadState: gamePadStates )
        {
            a = a || gamePadState.a == ButtonState::Pressed;
        }

        const bool space = keyboardState.isKeyPressed( KeyCode::Space );
        const bool up    = keyboardState.isKeyPressed( KeyCode::Up );

        return a || space || up;
    } );

    // Input::mapAxis( "Jump", []( std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState ) {
    //     float a = 0.0f;

    //    for ( auto& gamePadState: gamePadStates )
    //    {
    //        const auto state = gamePadState.getLastState();
    //        a += state.buttons.a ? 1.0f : 0.0f;
    //    }

    //    const auto keyState = keyboardState.getLastState();

    //    const float space = keyState.Space ? 1.0f : 0.0f;
    //    const float up    = keyState.Up ? 1.0f : 0.0f;

    //    return std::clamp( a + space + up, 0.0f, 1.0f );
    //} );

    project.loadFromFile( "assets/Pixel Adventure/Pixel Adventure.ldtk" );
    auto& world = project.getWorld();

    currentLevelId = 0;
    currentLevel   = Level( project, world, world.allLevels()[currentLevelId] );

    backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Blue.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Brown.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Gray.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Green.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Pink.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Purple.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );
    backgrounds.emplace_back( Background { "assets/Pixel Adventure/Background/Yellow.png", 1.0f, { 0.0f, 1.0f }, 0.3f } );

    currentBackground = backgrounds.begin();

    // Buttons
    {
        // Previous button.
        SpriteSheet sheet { "assets/Pixel Adventure/Menu/Buttons/Previous.png", {}, {}, BlendMode::AlphaBlend };
        previousButton = Button { sheet };
        previousButton.setCallback( [this] {
            onPreviousClicked();
        } );
        // Initially, we are on the first level, so hide the previous button.
        previousButton.enable( false );
    }
    {
        // Next button.
        SpriteSheet sheet { "assets/Pixel Adventure/Menu/Buttons/Next.png", {}, {}, BlendMode::AlphaBlend };
        nextButton = Button { sheet };
        nextButton.setCallback( [this] {
            onNextClicked();
        } );
    }
    {
        // Restart button.
        SpriteSheet sheet { "assets/Pixel Adventure/Menu/Buttons/Restart.png", {}, {}, BlendMode::AlphaBlend };
        restartButton = Button { sheet };
        restartButton.setCallback( [this] {
            onRestartClicked();
        } );
    }
}

void Game::Update()
{
    static double      totalTime = 0.0;
    static uint64_t    frames    = 0;
    static std::string fps       = "FPS: 0";

    timer.tick();
    ++frames;
    totalTime += timer.elapsedSeconds();
    if ( totalTime > 1.0 )
    {
        fps       = std::format( "FPS: {:.3f}", static_cast<double>( frames ) / totalTime );
        frames    = 0;
        totalTime = 0.0;
    }

    // Update and draw the background.
    currentBackground->update( timer );
    currentBackground->draw( image );

    auto elapsedTime = static_cast<float>( timer.elapsedSeconds() );
    do
    {
        // Update the input state.
        Input::update();

        currentLevel.update( std::min( elapsedTime, physicsTick ) );
        elapsedTime -= physicsTick;
    } while ( elapsedTime > 0.0f );

    currentLevel.draw( image );

    // Draw an FPS counter in the corner of the screen.
    image.drawText( arial20, 6, 20, fps, Color::Black );
    image.drawText( arial20, 4, 18, fps, Color::White );

#if _DEBUG
    // Draw some text at the mouse position.
    image.drawText( arial20, mousePos.x, mousePos.y, std::format( "({}, {})", mousePos.x, mousePos.y ), Color::White );
#endif

    // Draw the buttons
    restartButton.draw( image );
    nextButton.draw( image );
    previousButton.draw( image );

    // Simulate low frame rates.
    // timer.limitFPS( 25 );
}

void Game::processEvent( const sr::Event& _event )
{
    // Copy the event so we can modify it.
    Event event = _event;

    switch ( event.type )
    {
    case Event::None:
        break;
    case Event::Close:
        break;
    case Event::KeyPressed:
        break;
    case Event::KeyReleased:
        break;
    case Event::MouseMoved:
        onMouseMoved( event.mouseMove );
        break;
    case Event::MouseButtonPressed:
        break;
    case Event::MouseButtonReleased:
        break;
    case Event::MouseWheel:
        break;
    case Event::MouseHWheel:
        break;
    case Event::MouseEnter:
        break;
    case Event::MouseLeave:
        break;
    case Event::Resize:
        onResized( event.resize );
        break;
    case Event::EndResize:
        break;
    }

    previousButton.processEvents( event );
    nextButton.processEvents( event );
    restartButton.processEvents( event );
}

void Game::onMouseMoved( sr::MouseMovedEventArgs& args )
{
    // Compute the mouse position relative to the game screen (which can be scaled if the window is resized).
    const glm::vec2 scale {
        static_cast<float>( image.getWidth() ) / static_cast<float>( gameRect.width ),
        static_cast<float>( image.getHeight() ) / static_cast<float>( gameRect.height )
    };
    const glm::vec2 offset { gameRect.topLeft() };

    args.x = std::lround( ( static_cast<float>( args.x ) - offset.x ) * scale.x );
    args.y = std::lround( ( static_cast<float>( args.y ) - offset.y ) * scale.y );

    mousePos = { args.x, args.y };
}

void Game::onResized( sr::ResizeEventArgs& args )
{
    const float aspectRatio = static_cast<float>( image.getWidth() ) / static_cast<float>( image.getHeight() );
    const float scaleWidth  = static_cast<float>( args.width ) / static_cast<float>( image.getWidth() );
    const float scaleHeight = static_cast<float>( args.height ) / static_cast<float>( image.getHeight() );

    int width;
    int height;

    if ( scaleWidth < scaleHeight )
    {
        // Size according to the width.
        width  = args.width;
        height = static_cast<int>( static_cast<float>( width ) / aspectRatio );
    }
    else
    {
        // Size according to the height.
        height = args.height;
        width  = static_cast<int>( static_cast<float>( height ) * aspectRatio );
    }

    gameRect = {
        ( args.width - width ) / 2,
        ( args.height - height ) / 2,
        width, height
    };

    // Position the buttons in the top-right corner of the game screen.
    constexpr float spacing = 3.0f;
    float           y       = spacing;
    float           x       = static_cast<float>( image.getWidth() ) - spacing;

    x -= restartButton.getWidth() + spacing;
    restartButton.setTransform( Transform2D { { x, y } } );

    x -= nextButton.getWidth() + spacing;
    nextButton.setTransform( Transform2D { { x, y } } );

    x -= previousButton.getWidth() + spacing;
    previousButton.setTransform( Transform2D { { x, y } } );
}

void Game::onPreviousClicked()
{
    std::cout << "Previous Clicked!" << std::endl;
    assert( currentLevelId > 0 );

    --currentLevelId;

    auto& world  = project.getWorld();
    auto& levels = world.allLevels();
    
    currentLevel = Level { project, world, levels[currentLevelId] };

    previousButton.enable( currentLevelId != 0 );
}

void Game::onNextClicked()
{
    std::cout << "Next Clicked!" << std::endl;

    auto& world = project.getWorld();
    auto& levels = world.allLevels();

    currentLevelId = ( currentLevelId + 1 ) % levels.size();

    currentLevel = Level { project, world, levels[currentLevelId] };

    previousButton.enable( currentLevelId != 0 );
}

void Game::onRestartClicked()
{
    std::cout << "Restart Clicked!" << std::endl;
    if ( ++currentBackground == backgrounds.end() )
    {
        currentBackground = backgrounds.begin();
    }

    currentLevel.reset();
}
