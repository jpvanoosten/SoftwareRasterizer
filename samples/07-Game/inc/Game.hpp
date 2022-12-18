#pragma once

#include <Image.hpp>
#include <Timer.hpp>
#include <Font.hpp>

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

protected:
    sr::Image image;
    sr::Timer timer;

    // Fonts.
    sr::Font arial24;
    sr::Font arial56;
};

