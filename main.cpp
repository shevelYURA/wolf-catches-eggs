#include <SFML/Graphics.hpp>
#include "Eggs.h"

using namespace sf;

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    //окно
    RenderWindow window(VideoMode({ 1920, 1080 }), "Wolf Catches Eggs");
    
    Image icon;
    if (!icon.loadFromFile("image/icon.png")) {
        return 1;   //отсутствует иконка приложения
    }
    window.setIcon(icon);

    //------------------------------------------------------------------------------------------//
    
    //Объекты:
    
    //Волк
    Texture texWolf;
    if (!texWolf.loadFromFile("image/wolfTexture.png")) {
        return 2;
    }
    RectangleShape WOLF(Vector2f(50, 150));
    WOLF.setTexture(&texWolf);
    WOLF.setPosition(Vector2f(910, 1000));

    //Корзинка
    Texture texBasket;
    if (!texBasket.loadFromFile("image/basketTexture.png")) {
        return 3;
    }
    RectangleShape Basket(Vector2f(50, 15));
    Basket.setTexture(&texBasket);
    Basket.setPosition(Vector2f(WOLF.getPosition().x, WOLF.getPosition().y - 15*9));

    //Яйца
    Eggs eggs[10];
    int count_eggs = 10;

    //-----------------------------------------------------------------------------------------//

    //переменные
    bool GameOver = false;
    Clock clock;
    float time, timeEggs;

    const float WOLF_SPEED = 0.5f;

    while (window.isOpen())
    {
        time = clock.getElapsedTime().asMicroseconds();
        timeEggs = time / 3000;
        time = time / 6000;
        clock.restart();

        while (const std::optional event = window.pollEvent())
        {
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
        }

        for (int i = 0; i < count_eggs; i++)
        {
            eggs[i].move(timeEggs);

            if (eggs[i].collision(Basket.getGlobalBounds())) {
                eggs[i].restart();
            }
        }
        window.clear();
        window.draw(WOLF);
        window.draw(Basket);
        for (int i = 0; i < count_eggs; i++) eggs[i].draw(window);
        window.display();
    }
}