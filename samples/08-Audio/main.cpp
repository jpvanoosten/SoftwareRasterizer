#include "Ball.hpp"

#include <Device.hpp>
#include <Font.hpp>
#include <Image.hpp>
#include <Math/Transform2D.hpp>
#include <Timer.hpp>
#include <Window.hpp>

#include <iostream>
#include <random>

using namespace sr;
using namespace Math;

int main( int argc, char* argv[] )
{
    // Parse command-line arguments.
    if ( argc > 1 )
    {
        for ( int i = 0; i < argc; ++i )
        {
            if ( strcmp( argv[i], "-cwd" ) == 0 )
            {
                std::string workingDirectory = argv[++i];
                std::filesystem::current_path( workingDirectory );
            }
        }
    }

    const int WINDOW_WIDTH  = 800;
    const int WINDOW_HEIGHT = 600;

    Window window { L"08-Audio", WINDOW_WIDTH, WINDOW_HEIGHT };
    Image  image { WINDOW_WIDTH, WINDOW_HEIGHT };

    window.show();

    Ball ball { { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f } };
    ball.setVelocity( glm::vec2 { 1, -1 } * 200.0f );

    // Set the audio listener to be in the center of the screen.
    Audio::Device::getListener().setPosition( { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f, 0.0f } );

    // Load some bouncing sounds.
    std::vector<Audio::Sound> bounceSounds;
    bounceSounds.emplace_back( "assets/sounds/bounce-1.wav", Audio::Sound::Type::Sound );
    bounceSounds.emplace_back( "assets/sounds/bounce-2.wav", Audio::Sound::Type::Sound );
    bounceSounds.emplace_back( "assets/sounds/bounce-3.wav", Audio::Sound::Type::Sound );
    bounceSounds.emplace_back( "assets/sounds/bounce-4.wav", Audio::Sound::Type::Sound );

    for ( auto& sound: bounceSounds )
    {
        // The default attenuation model is Inverse, but the linear attenuation model sounds better for this demo.
        sound.setAttenuationModel( Audio::Sound::AttenuationModel::Linear );
    }

    std::minstd_rand                rng { std::random_device()() };
    std::uniform_int_distribution<> dist { 0, static_cast<int>( bounceSounds.size() - 1 ) };

    Audio::Sound bgMusic { "assets/sounds/piano-loops.mp3", Audio::Sound::Type::Music };
    bgMusic.setVolume( 0.2f );  // It's too loud!

    bgMusic.play();

    Timer       timer;
    double      totalTime  = 0.0;
    uint64_t    frameCount = 0ull;
    std::string fps        = "FPS: 0";

    while ( window )
    {
        ball.update( static_cast<float>( timer.elapsedSeconds() ) );

        // Check for collisions with the edges of the screen.
        Circle    c   = ball.getCircle();
        glm::vec2 vel = ball.getVelocity();
        if ( c.left() <= 0.0f )
        {
            c.center.x = c.radius;
            vel.x *= -1.0f;
            // Play a random bounce sound.
            bounceSounds[dist( rng )].restart();
        }
        else if ( c.right() >= static_cast<float>( image.getWidth() ) )
        {
            c.center.x = static_cast<float>( image.getWidth() ) - c.radius;
            vel.x *= -1.0f;
            bounceSounds[dist( rng )].restart();
        }
        if ( c.top() <= 0.0f )
        {
            c.center.y = c.radius;
            vel.y *= -1.0f;
            bounceSounds[dist( rng )].restart();
        }
        else if ( c.bottom() >= static_cast<float>( image.getHeight() ) )
        {
            c.center.y = static_cast<float>( image.getHeight() ) - c.radius;
            vel.y *= -1.0f;
            bounceSounds[dist( rng )].restart();
        }
        ball.setCircle( c );
        ball.setVelocity( vel );

        // Update the sound positions.
        for ( auto& sound: bounceSounds )
        {
            // Update the position of the sound to enable 3D spatialization of the sound effect.
            sound.setPosition( glm::vec3 { ball.getPosition(), 0 } );
        }

        image.clear( Color::Black );

        ball.draw( image );

        image.drawText( Font::Default, 10, 10, fps, Color::White );

        window.present( image );

        Event e;
        while ( window.popEvent( e ) )
        {
            switch ( e.type )
            {
            case Event::Close:
                window.destroy();
                break;
            case Event::KeyPressed:
                switch ( e.key.code )
                {
                case KeyCode::Escape:
                    window.destroy();
                    break;
                case KeyCode::V:
                    window.setVSync( !window.isVSync() );
                    std::cout << "Vsync: " << window.isVSync() << std::endl;
                    break;
                }
                break;
                // case Event::Resize:
                //     std::cout << std::format( "Resize: {},{}\n", e.resize.width, e.resize.height );
                //     image.resize( e.resize.width, e.resize.height );
                //     image.clear( Color::Black );
                //     break;
            }
        }

        timer.tick();
        ++frameCount;

        totalTime += timer.elapsedSeconds();
        if ( totalTime > 1.0 )
        {
            fps = std::format( "FPS: {:.3f}", static_cast<double>( frameCount ) / totalTime );

            std::cout << fps << std::endl;

            frameCount = 0;
            totalTime  = 0.0;
        }
    }
}