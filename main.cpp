#include <SFML/Graphics.hpp>
#include "Eggs.h"

using namespace sf;

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    //окно
    RenderWindow window(VideoMode({ 1280, 720 }), "Wolf Catches Eggs");
    
    Image icon;
    if (!icon.loadFromFile("image/icon.png")) {
        return 1;
    }
    window.setIcon(icon);

    //------------------------------------------------------------------------------------------//
    
    //Объекты
    
    //Волк
    Texture texWolf;
    if (!texWolf.loadFromFile("image/wolfTexture.png")) {
        return 2;
    }
    RectangleShape WOLF(Vector2f(100, 300));
    WOLF.setTexture(&texWolf);
    WOLF.setPosition(Vector2f(590, 320));

    //Корзинка
    Texture texBasket;
    if (!texBasket.loadFromFile("image/basketTexture.png")) {
        return 3;
    }
    RectangleShape Basket(Vector2f(150, 50));
    Basket.setTexture(&texBasket);
    Basket.setPosition(Vector2f(565, 400));

    //Яйца
    Eggs eggs[3];
    int count_eggs = 3;

    //-----------------------------------------------------------------------------------------//

    //переменные
    bool GameOver = false;
    Clock clock;
    float time, timeEggs;

    //
    

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            time = clock.getElapsedTime().asMicroseconds();
            timeEggs = time / 3000;
            time = time / 6000;
            clock.restart();

            if (event->is <Event::Closed>())
                window.close();

            if (event->is <Event::KeyPressed>()) {
                const auto& keyEvent = event->getIf<Event::KeyPressed>();
                if (keyEvent->code == Keyboard::Key::A) {
                    WOLF.setPosition(Vector2f(300, 300));
                    Basket.setPosition(Vector2f(WOLF.getPosition().x - 110, Basket.getPosition().y));
                }
                if (keyEvent->code == Keyboard::Key::D) {
                    WOLF.setPosition(Vector2f(820, 300));
                    Basket.setPosition(Vector2f(WOLF.getPosition().x + 90, Basket.getPosition().y));
                }
                if (keyEvent->code == Keyboard::Key::W) {
                    Basket.setPosition(Vector2f(Basket.getPosition().x, 300));
                }
                if (keyEvent->code == Keyboard::Key::S) {
                    Basket.setPosition(Vector2f(Basket.getPosition().x, 400));
                }
            }
            for (int i = 0; i < count_eggs; i++) eggs[i].move(timeEggs);
        }
        window.clear();
        window.draw(WOLF);
        window.draw(Basket);
        for (int i = 0; i < count_eggs; i++) eggs[i].draw(window);
        window.display();
    }
}