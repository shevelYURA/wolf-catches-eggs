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
#include "EggFallBoost.h"  // ← ДОБАВЛЕНО

using namespace sf;

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    //----------------------------------------------------------------------------------//

    RenderWindow window(VideoMode({ 1920, 1080 }), "Wolf Catches Eggs");
    window.setFramerateLimit(144);

    // Загрузка иконки
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

    //----------------------------------------------------------------------------------//

    Player player;
    std::vector<std::unique_ptr<FallingObject>> fallingObjects;
    const int count_eggs = 7;
    
    // СОЗДАНИЕ ЯИЦ С 20% ШАНСОМ ЗОЛОТОГО
    for (int i = 0; i < count_eggs; ++i) {
        auto egg = std::make_unique<Egg>();
        
        // 20% шанс сделать яйцо золотым
        if (rand() % 100 < 20) {
            egg->setGolden(true);
        }
        
        fallingObjects.push_back(std::move(egg));
    }
    
    Scorer scoreCounter;
    HealthBar healthBar;
    Boss boss;
    BossHealthBar bossHealthBar;
    bool bossDefeated = false;

    Clock clock;

    std::vector<Egg*> boostEggs;  // ← ДОБАВЛЕНО (для яиц из буста)

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds() / 600000.0f;
        clock.restart();

        // ← ДОБАВЛЕНО (обновление буста)
        EggFallBoost::update(time, boostEggs);

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

        // ОБРАБОТКА СТОЛКНОВЕНИЙ С ЯЙЦАМИ
        for (auto& obj : fallingObjects) {
            obj->move(time);
            if (obj->collision(player.getBasketBounds())) {
                if (auto* egg = dynamic_cast<Egg*>(obj.get())) {
                    obj->restart();
                    
                    if (egg->getGolden()) {
                        scoreCounter.addScore(1500);  // Золотое: 1500 очков
                    } else {
                        scoreCounter.addScore(500);   // Обычное: 500 очков
                    }
                }
            }
        }

        // ← ДОБАВЛЕНО (временная активация буста по клавише B для теста)
        if (Keyboard::isKeyPressed(Keyboard::Key::B)) {
            EggFallBoost::activate(8.0f);
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

        // ← ДОБАВЛЕНО (отрисовка яиц из буста)
        for (auto* egg : boostEggs) {
            window.draw(egg->shape);
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

                // ← ДОБАВЛЕНО (очистка яиц буста при рестарте)
                for (auto* egg : boostEggs) {
                    delete egg;
                }
                boostEggs.clear();
            }
        }
        bossDialog.draw(window);
        window.display();

    }

    return 0;
}
