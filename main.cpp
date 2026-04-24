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
#include "FirebaseManager.h"
#include "PlayerNameManager.h"

using namespace sf;

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    RenderWindow window(VideoMode({ 1920, 1080 }), "Wolf Catches Eggs");
    window.setFramerateLimit(144);

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

    PlayerNameManager nameManager;
    std::string playerName = nameManager.getName();
    bool waitingForName = false;
    std::string inputName = "";

    if (playerName == "WolfPlayer") {
        waitingForName = true;
    }

    FirebaseManager firebase;
    bool scoreSaved = false;

    Player player;
    std::vector<std::unique_ptr<FallingObject>> fallingObjects;
    const int count_eggs = 7;
    
    // СОЗДАНИЕ ЯИЦ С 20% ШАНСОМ ЗОЛОТОГО
    for (int i = 0; i < count_eggs; ++i) {
        auto egg = std::make_unique<Egg>();
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

    // БУСТ: ДВОЙНЫЕ ОЧКИ
    bool doublePoints = false;
    float doublePointsTimer = 0.0f;

    Clock clock;
    Font& font = ResourceManager::getFont(0);

    Text enterNameText(font);
    enterNameText.setString("ENTER YOUR NAME: " + inputName + "_");
    enterNameText.setCharacterSize(48);
    enterNameText.setFillColor(Color::White);
    enterNameText.setOutlineColor(Color::Black);
    enterNameText.setOutlineThickness(2);
    enterNameText.setPosition(Vector2f(960, 540));
    enterNameText.setOrigin(Vector2f(enterNameText.getLocalBounds().size.x / 2, enterNameText.getLocalBounds().size.y / 2));

    Text bestText(font);
    bestText.setCharacterSize(28);
    bestText.setFillColor(Color::Yellow);
    bestText.setOutlineColor(Color::Black);
    bestText.setOutlineThickness(1);
    bestText.setPosition(Vector2f(250, 25));

    Text boostText(font);
    boostText.setCharacterSize(28);
    boostText.setFillColor(Color(255, 215, 0)); // Золотой цвет
    boostText.setOutlineColor(Color::Black);
    boostText.setOutlineThickness(1);
    boostText.setPosition(Vector2f(250, 55));

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds() / 600000.0f;
        clock.restart();
        if (waitingForName) {
            while (const std::optional event = window.pollEvent())
            {
                if (event->is<Event::Closed>())
                    window.close();

                if (event->is<Event::TextEntered>()) {
                    char c = static_cast<char>(event->getIf<Event::TextEntered>()->unicode);
                    if (c == '\b' && !inputName.empty()) {
                        inputName.pop_back();
                    }
                    else if (c == '\r' && !inputName.empty()) {
                        playerName = inputName;
                        nameManager.setName(playerName);
                        waitingForName = false;
                    }
                    else if (isalnum(c) || c == ' ') {
                        if (inputName.length() < 20) inputName += c;
                    }
                }
            }

            enterNameText.setString("ENTER YOUR NAME: " + inputName + "_");
            enterNameText.setOrigin(Vector2f(enterNameText.getLocalBounds().size.x / 2, enterNameText.getLocalBounds().size.y / 2));

            window.clear(Color(0, 0, 0, 255));
            window.draw(enterNameText);
            window.display();
            continue;
        }

        bestText.setString("BEST: " + std::to_string(nameManager.getPersonalBest()));

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();

            // ---------- Активация буста по клавише P ----------
            if (const auto* keyPressed = event->getIf<Event::KeyPressed>())
            {
                if (keyPressed->code == Keyboard::Key::P)
                {
                    if (!doublePoints)
                    {
                        doublePoints = true;
                        doublePointsTimer = 5.0f;
                    }
                }
            }
            // -----------------------------------------------------------------
        }

        static Dialog bossDialog;
        static bool dialogShown = false;
        static bool waitingForChoice = false;
        static bool playerChoseStop = false;

        static bool dialogInitialized = false;
        if (!dialogInitialized) {
            bossDialog.setCallbacks(
                [&]() {
                    playerChoseStop = true;
                    waitingForChoice = false;
                },
                [&]() {
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
            player.takeDamage(100);
            playerChoseStop = false;
        }

        if (player.isAlive()) {
            player.update(time, window);
        }

        for (auto& obj : fallingObjects) {
            obj->move(time);
            if (obj->collision(player.getBasketBounds())) {
                if (auto* egg = dynamic_cast<Egg*>(obj.get())) {
                    
                    int points = egg->getGolden() ? 1500 : 500;

                    if (doublePoints) {
                        points *= 2;
                        //scoreCounter.addScore(500);   // Обычное: 500 очков
                    }

                    scoreCounter.addScore(points);
                    obj->restart();
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

        // Таймер для двойных очков
                // Таймер для двойных очков
        if (doublePoints) {
            doublePointsTimer -= time;
            if (doublePointsTimer <= 0.0f) {
                doublePoints = false;
            }
            // Обновление текста буста
            std::string boostStr = "2X POINTS! " + std::to_string(static_cast<int>(doublePointsTimer)) + "s";
            if (doublePointsTimer < 1.0f) {
                // Мерцание в последнюю секунду
                if (static_cast<int>(doublePointsTimer * 10) % 2 == 0) {
                    boostText.setFillColor(Color::Red);
                }
                else {
                    boostText.setFillColor(Color::Yellow);
                }
            }
            else {
                boostText.setFillColor(Color(255, 215, 0));
            }
            boostText.setString(boostStr);
        }
        else {
            boostText.setString("");
        }

        healthBar.update(player.getHealth());

        window.clear();
        player.draw(window);
        for (auto& obj : fallingObjects) {
            obj->draw(window);
        }
        scoreCounter.draw(window);
        healthBar.draw(window);
        window.draw(bestText);
        window.draw(boostText);

        if (boss.isActive()) {
            boss.draw(window);
            bossHealthBar.draw(window);
        }

        if (!player.isAlive()) {
            if (!scoreSaved) {
                firebase.saveScore(playerName, scoreCounter.getScore());
                nameManager.updatePersonalBest(scoreCounter.getScore());
                scoreSaved = true;
            }

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
                doublePoints = false;
                doublePointsTimer = 0.0f;
                scoreSaved = false;
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