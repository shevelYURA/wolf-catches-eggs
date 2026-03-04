#include <SFML/Graphics.hpp>
#include "Eggs.h"
#include "Player.h"
#include <ctime>
#include <cstdlib>

using namespace sf;

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    //----------------------------------------------------------------------------------//

    RenderWindow window(VideoMode({ 1920, 1080 }), "Wolf Catches Eggs");

    Image icon;
    if (!icon.loadFromFile("image/icon.png")) {
        return 1;
    }
    window.setIcon(icon);

    //----------------------------------------------------------------------------------//

    Player player;
    Eggs eggs[7];
    const int count_eggs = 7;

    //----------------------------------------------------------------------------------//

    Clock clock;
    float time;

    //----------------------------------------------------------------------------------//

    while (window.isOpen())
    {
        time = clock.getElapsedTime().asMicroseconds();
        time = time / 600000;
        clock.restart();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is <Event::Closed>())
                window.close();
        }

        player.update(time);

        for (int i = 0; i < count_eggs; i++)
        {
            eggs[i].move(time);
            if (eggs[i].collision(player.getBasketBounds())) {
                eggs[i].restart();
            }
        }

        window.clear();
        player.draw(window);
        for (int i = 0; i < count_eggs; i++) eggs[i].draw(window);
        window.display();
    }
}