#include <SFML/Graphics.hpp>
#include "Eggs.h"
#include "Bombs.h"
#include "Player.h"
#include "Scorer.h"
#include "HealthBar.h"
#include "PlayersAttack.h"
#include "Boss.h"
#include "BossHealthBar.h"
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
    window.setFramerateLimit(144);

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
    Scorer scoreCounter;
    HealthBar healthBar;

    Boss boss;
    BossHealthBar bossHealthBar;
    bool bossDefeated = false;

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

        if (scoreCounter.getScore() >= 5000 && !boss.isActive() && !bossDefeated) {
            boss.activate();
            bossHealthBar.setActive(true);
        }

        if (player.isAlive()) {
            player.update(time, window);
        }

        for (auto& obj : fallingObjects) {
            obj->move(time);
            if (obj->collision(player.getBasketBounds())) {
                if (dynamic_cast<Egg*>(obj.get())) {
                    // 20% шанс, что яйцо золотое
                    bool isGolden = (rand() % 100 < 20);
    
                    obj->restart();
    
                    if (isGolden) {
                        scoreCounter.addScore(25000);  // Золотое яйцо: x5 (25000)
                    } else {
                        scoreCounter.addScore(5000);   // Обычное яйцо: 5000 очков
                    }
                }
            }
        }

        if (boss.isActive()) {
            boss.update(time, window, player.getBottomCenter(), player.getBounds());
            bossHealthBar.update(boss.getHealth());

            for (auto& bullet : boss.getAttackSystem().getBullets()) {
                if (bullet->checkPlayerCollision(player.getBounds())) {
                    player.takeDamage(20);
                }
            }

            if (player.getAttack().isInFlight()) {
                if (player.getAttack().getPosition().findIntersection(boss.getBounds()).has_value()) {
                    boss.takeDamage(10);
                    player.getAttack().stop();

                    if (!boss.isAlive()) {
                        boss.reset();
                        bossHealthBar.setActive(false);
                        scoreCounter.addScore(5000);
                        bossDefeated = true;
                    }
                }
            }
        }

        if (player.getAttack().isInFlight()) {
            for (auto& obj : fallingObjects) {
                if (obj->collision(player.getAttack().getPosition())) {
                    if (dynamic_cast<Bomb*>(obj.get())) {
                        obj->restart();
                        player.getAttack().stop();
                        scoreCounter.addScore(100);
                        break;
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

        if (boss.isActive()) {
            boss.draw(window);
            bossHealthBar.draw(window);
        }

        if (!player.isAlive()) {
            // Временный текст для сообщения о смерти
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
                    boss.reset();
                    bossHealthBar.setActive(false);
                    bossDefeated = false;

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
