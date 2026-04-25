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
#include "EggFallBoost.h"
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

    

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds() / 600000.0f;
        clock.restart();

        // Обновление буста
        EggFallBoost::update(time);

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
                    obj->restart();
                    if (egg->getGolden()) {
                        scoreCounter.addScore(1500);
                    }
                    else {
                        scoreCounter.addScore(500);
                    }
                }
            }
        }

        // Временная активация буста по клавише B для теста
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

        // Отрисовка яиц из буста (только активные)
for (int i = 0; i < (int)EggFallBoost::getEggs().size(); i++) {
    if (EggFallBoost::isEggAlive(i)) {
        EggFallBoost::getEggs()[i]->draw(window);
    }
}

        scoreCounter.draw(window);
        healthBar.draw(window);
        window.draw(bestText);

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
    playerChoseStop = false;
    scoreSaved = false;

    for (auto& obj : fallingObjects) {
        obj->restart();
    }

    // Очистка яиц буста при рестарте
    EggFallBoost::deactivate();
}
               
   
        }
        bossDialog.draw(window);
        window.display();
    }

    return 0;
}
