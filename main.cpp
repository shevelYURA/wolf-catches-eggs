#include <SFML/Graphics.hpp>
#include "Eggs.h"
#include "Bombs.h"
#include "Player.h"
#include "Scorer.h"
#include "HealthBar.h"
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
    Bombs bombs[3];
    const int count_eggs = 7;
    const int count_bombs = 3;
    Scorer scoreCounter;
    HealthBar healthBar;

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

        if (player.isAlive()) {
            player.update(time);
        }

        for (int i = 0; i < count_eggs; i++)
        {
            eggs[i].move(time);
            if (eggs[i].collision(player.getBasketBounds())) {
                eggs[i].restart();
                scoreCounter.addScore(500);
            }
        }

        for (int i = 0; i < count_bombs; i++)
        {
            bombs[i].move(time);
            if (player.isAlive() && bombs[i].collision(player.getBasketBounds())) {
                bombs[i].restart();
                player.takeDamage(20);
            }
        }

        healthBar.update(player.getHealth());

        window.clear();
        player.draw(window);
        for (int i = 0; i < count_eggs; i++) eggs[i].draw(window);
        for (int i = 0; i < count_bombs; i++) bombs[i].draw(window);
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
                    for (int i = 0; i < count_eggs; i++) {
                        eggs[i].restart();
                    }
                    for (int i = 0; i < count_bombs; i++) {
                        bombs[i].restart();
                    }
                }
            }
        }

        window.display();
    }
}