#include <cstdlib>
#include <Engine.hpp>
#include <Window.hpp>

using namespace Rasterizer;

int main(int argc, char* argv[])
{
    Engine& engine = Engine::Get();

    Window window{ "01-ClearScreen", 800, 600 };

    window.show();


    
    system("PAUSE");
}