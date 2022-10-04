#include <Image.hpp>
#include <Window.hpp>
#include <Timer.hpp>

#include <iostream>

using namespace sr;

int main(int argc, char* argv[])
{
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    Window window{ L"01-ClearScreen", WINDOW_WIDTH, WINDOW_HEIGHT };

    Image image{ WINDOW_WIDTH, WINDOW_HEIGHT };
    auto ptr = image.data();
//    image.clear(Color::Black);
    for (int y = 0; y < WINDOW_HEIGHT; ++y)
    {
        for (int x = 0; x < WINDOW_WIDTH; ++x)
        {
            auto r = static_cast<uint8_t>(x / static_cast<float>(WINDOW_WIDTH) * 255);
            auto g = static_cast<uint8_t>(y / static_cast<float>(WINDOW_HEIGHT) * 255);
            ptr[y * WINDOW_WIDTH + x] = Color{ r, g, 0 };
        }
    }

    window.show();

    Timer timer;
    double totalTime = 0.0;
    uint64_t frameCount = 0ull;

    while (window)
    {
        window.present(image);

        Event e;
        while (window.popEvent(e))
        {
            switch (e.type)
            {
            case Event::Close:
                window.destroy();
                break;
            }
        }

        timer.tick();
        ++frameCount;

        totalTime += timer.elapsedSeconds();
        if (totalTime > 1.0)
        {
            double fps = frameCount / totalTime;
            std::cout << "FPS: " << fps << std::endl;

            frameCount = 0;
            totalTime = 0.0;
        }
    }
}