#include <Image.hpp>
#include <Window.hpp>

using namespace sr;

int main(int argc, char* argv[])
{
    Window window{ L"01-ClearScreen", 800, 600 };
    Image image{ 800, 600 };
    image.clear(Color::Black);

    window.show();

    while(window)
    {
        window.present(image);

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
    }
}