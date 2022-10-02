#include <Window.hpp>

using namespace Rasterizer;

int main(int argc, char* argv[])
{
    Window window{ L"01-ClearScreen", 800, 600 };

    window.show();

    while(window)
    {
        Event e;
        while(window.popEvent(e))
        {
            switch (e.type)
            {
            case Event::Close:
                window.destroy();
                break;
            }
        }

        // TODO: Render
    }
}