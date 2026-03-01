#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    RenderWindow window(VideoMode({ 1280, 720 }), "Wolf Catches Eggs");
    
    Image icon;
    if (!icon.loadFromFile("image/icon.png")) {
        return 1;   //отсутствует иконка приложения
    }
    window.setIcon(icon);

    //"ВОЛК"
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
    Texture texEggs;
    if (!texEggs.loadFromFile("image/eggsTexture.jpg")) {
        return 3;
    }
    CircleShape Eggs(25.0f);
    Eggs.setTexture(&texEggs);
    Eggs.setPosition(Vector2f(0, 0));

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is <Event::Closed>())
                window.close();

            if (event->is <Event::KeyPressed>()) {
                const auto& keyEvent = event->getIf<Event::KeyPressed>();
                if (keyEvent->code == Keyboard::Key::A) {
                    WOLF.setPosition(Vector2f(300, 300));
                    Basket.setPosition(Vector2f(210, Basket.getPosition().y));
                }
                if (keyEvent->code == Keyboard::Key::D) {
                    WOLF.setPosition(Vector2f(820, 300));
                    Basket.setPosition(Vector2f(910, Basket.getPosition().y));
                }
                if (keyEvent->code == Keyboard::Key::W) {
                    Basket.setPosition(Vector2f(Basket.getPosition().x, 300));
                }
                if (keyEvent->code == Keyboard::Key::S) {
                    Basket.setPosition(Vector2f(Basket.getPosition().x, 400));
                }
            }
        }
        window.clear();
        window.draw(WOLF);
        window.draw(Basket);
        window.draw(Eggs);
        window.display();
    }
}