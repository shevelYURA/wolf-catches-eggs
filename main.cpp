#include <SFML/Graphics.hpp>
#include "Eggs.h"
#include "Bombs.h"
#include "Player.h"
#include "Scorer.h"
#include "HealthBar.h"
#include <ctime>
#include <cstdlib>
#include <vector>
#include <memory>

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
    std::vector<std::unique_ptr<FallingObject>> fallingObjects;
    const int count_eggs = 7;
    for (int i = 0; i < count_eggs; ++i) {
        fallingObjects.push_back(std::make_unique<Egg>());
    }
    const int count_bombs = 3;
    for (int i = 0; i < count_bombs; ++i) {
        fallingObjects.push_back(std::make_unique<Bomb>());
    }
    Scorer scoreCounter;
    HealthBar healthBar;

    //----------------------------------------------------------------------------------//

    Clock clock;

    //----------------------------------------------------------------------------------//

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds() / 600000.0f;
        clock.restart();
            
        while (const std::optional event = window.pollEvent())
        {
            if (event->is <Event::Closed>())
                window.close();
        }

        if (player.isAlive()) {
            player.update(time);
        }

        for (auto& obj : fallingObjects) {
            obj->move(time);

            // Проверяем коллизию с игроком
            if (obj->collision(player.getBasketBounds())) {
                // Определяем тип объекта через dynamic_cast
                if (dynamic_cast<Egg*>(obj.get())) {
                    obj->restart();
                    scoreCounter.addScore(500);
                }
                else if (dynamic_cast<Bomb*>(obj.get())) {
                    if (player.isAlive()) {
                        obj->restart();
                        player.takeDamage(20);
                    }
                }
            }
        }

        healthBar.update(player.getHealth());

        window.clear();
        player.draw(window);
        for (auto& obj : fallingObjects) {
            obj->draw(window);
        }
        scoreCounter.draw(window);
        healthBar.draw(window);

        if (!player.isAlive()) {
            // Создаем временный текст для сообщения о смерти
            Font font;
            if (font.openFromFile("image/ARCADECLASSIC.ttf")) {
                Text gameOverText(font);
                gameOverText.setString("GAME OVER! Press R to restart");
                gameOverText.setCharacterSize(72);
                gameOverText.setFillColor(Color::Red);
                gameOverText.setOutlineColor(Color::Black);
                gameOverText.setOutlineThickness(3);

                // Центрируем текст
                FloatRect textBounds = gameOverText.getLocalBounds();
                gameOverText.setOrigin(Vector2f(textBounds.size.x / 2, textBounds.size.y / 2));
                gameOverText.setPosition(Vector2f(960, 540));

                window.draw(gameOverText);

                // Проверка нажатия R для рестарта
                if (Keyboard::isKeyPressed(Keyboard::Key::R)) {
                    player.reset();
                    scoreCounter.reset();

                    // Перезапускаем все яйца и бомбы
                    for (auto& obj : fallingObjects) {
                        obj->restart();
                    }
                }
            }
        }

        window.display();
    }
}