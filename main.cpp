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
#include "ResourceManager.h"
#include "dialog.h"

using namespace sf;

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    //----------------------------------------------------------------------------------//

    RenderWindow window(VideoMode({ 1920, 1080 }), "Wolf Catches Eggs");
    window.setFramerateLimit(144);

    // Загрузка иконки
    // Загрузка иконки с обработкой ошибок
    HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(IDB_PNG6), L"PNG");
    if (hRes) {
        HGLOBAL hData = LoadResource(NULL, hRes);
        if (hData) {
            void* pData = LockResource(hData);
            DWORD size = SizeofResource(NULL, hRes);
            if (pData && size > 0) {
                Image icon;
                if (icon.loadFromMemory(pData, size)) {
                    window.setIcon(icon);
                }
            }
        }
    }
    // Если иконка не загрузилась - просто продолжаем без неё

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

    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds() / 600000.0f;
        clock.restart();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }

        // Диалог перед боссом
        static Dialog bossDialog;
        static bool dialogShown = false;
        static bool waitingForChoice = false;
        static bool playerChoseStop = false;

        // Инициализация диалога (выполняется один раз)
        static bool dialogInitialized = false;
        if (!dialogInitialized) {
            bossDialog.setCallbacks(
                [&]() {
                    // Выбрал STOP - рестарт игры
                    playerChoseStop = true;
                    waitingForChoice = false;
                },
                [&]() {
                    // Выбрал CONTINUE - появляется босс
                    boss.activate();
                    bossHealthBar.setActive(true);
                    waitingForChoice = false;
                }
            );
            dialogInitialized = true;
        }

        if (scoreCounter.getScore() >= 5000 && !boss.isActive() && !bossDefeated && !dialogShown && !playerChoseStop) {
            bossDialog.show();
            dialogShown = true;
            waitingForChoice = true;
        }

        if (bossDialog.isActive()) {
            bossDialog.handleInput(window);
        }

        if (playerChoseStop) {
            player.takeDamage(100); // Убиваем игрока для показа game over
            playerChoseStop = false;
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
                        scoreCounter.addScore(2500);  // Золотое яйцо: x5 (2500)
                    } else {
                        scoreCounter.addScore(500);   // Обычное яйцо: 500 очков
                    }
                }
            }
        }

        if (boss.isActive()) {
            boss.update(time, window, player.getBottomCenter(), player.getBounds());
            bossHealthBar.update(boss.getHealth());

            for (auto& bullet : boss.getAttackSystem().getBullets()) {
                if (bullet->checkPlayerCollision(player.getBounds())) {
                    player.takeDamage(25);
                }
            }

            if (player.getAttack().isInFlight()) {
                if (player.getAttack().getPosition().findIntersection(boss.getBounds()).has_value()) {
                    boss.takeDamage(10);
                    player.getAttack().stop();

                    if (!boss.isAlive()) {
                        boss.reset();
                        bossHealthBar.setActive(false);
                        scoreCounter.addScore(50000);
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
            Font& font = ResourceManager::getFont(0);
            Text gameOverText(font);
            gameOverText.setString("GAME OVER! Press R to restart");
            gameOverText.setCharacterSize(72);
            gameOverText.setFillColor(Color::Red);
            gameOverText.setOutlineColor(Color::Black);
            gameOverText.setOutlineThickness(3);

            FloatRect textBounds = gameOverText.getLocalBounds();
            gameOverText.setOrigin(Vector2f(textBounds.size.x / 2, textBounds.size.y / 2));
            gameOverText.setPosition(Vector2f(960, 540));

            window.draw(gameOverText);

            if (Keyboard::isKeyPressed(Keyboard::Key::R)) {
                player.reset();
                scoreCounter.reset();
                boss.reset();
                bossHealthBar.setActive(false);
                bossDefeated = false;
                dialogShown = false;
                waitingForChoice = false;
                playerChoseStop = false;

                for (auto& obj : fallingObjects) {
                    obj->restart();
                }
            }
        }
        bossDialog.draw(window);
        window.display();

    }

    return 0;
}