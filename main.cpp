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
#include "screenConfig.h"
#include "PowerUpManager.h"

using namespace sf;

void restartGame(Player& player, Scorer& scoreCounter, Boss& boss,
    BossHealthBar& bossHealthBar, bool& bossDefeated,
    bool& dialogShown, bool& waitingForChoice,
    bool& doublePoints, float& doublePointsTimer,
    bool& scoreSaved, bool& playerChoseStop,
    bool& eggRainActive, float& eggRainTimer,
    PowerUpManager& powerUpManager, int& extraEggsCount,
    std::vector<std::unique_ptr<FallingObject>>& fallingObjects,
    int count_eggs, bool& showVictory) {
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

    eggRainActive = false;
    eggRainTimer = 0.0f;
    powerUpManager.reset();
    extraEggsCount = 0;

    while (fallingObjects.size() > static_cast<size_t>(count_eggs)) {
        fallingObjects.pop_back();
    }

    for (auto& obj : fallingObjects) {
        if (auto* egg = dynamic_cast<Egg*>(obj.get())) {
            egg->disableRainMode();
        }
        obj->restart();
    }

    showVictory = false;
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    VideoMode desktopMode = VideoMode::getDesktopMode();
    unsigned int screenWidth = static_cast<unsigned int>(desktopMode.size.x * 0.95f);
    unsigned int screenHeight = static_cast<unsigned int>(desktopMode.size.y * 0.9f);
    ScreenConfig::init(screenWidth, screenHeight);

    RenderWindow window(VideoMode({ screenWidth, screenHeight }), "Wolf Catches Eggs", Style::Default);
    window.setFramerateLimit(144);

    int windowPosX = (desktopMode.size.x - screenWidth) / 2;
    int windowPosY = (desktopMode.size.y - screenHeight) / 2;
    window.setPosition(Vector2i(windowPosX, windowPosY));
    
    // ========== ЗАГРУЗКА ФОНА ==========
    Texture& backgroundTexture = ResourceManager::getTexture(IDB_BACKGROUND);
    Sprite backgroundSprite(backgroundTexture);

    Vector2u windowSize = window.getSize();
    Vector2u textureSize = backgroundTexture.getSize();
    backgroundSprite.setScale(Vector2f(
        (float)windowSize.x / textureSize.x,
        (float)windowSize.y / textureSize.y
    ));
    // ===================================
    
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
    int extraEggsCount = 0;
    
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

    bool doublePoints = false;
    float doublePointsTimer = 0.0f;

    PowerUpManager powerUpManager;
    bool eggRainActive = false;
    float eggRainTimer = 0.0f;
    const float EGG_RAIN_DURATION = 12.0f;

    Clock clock;
    Font& font = ResourceManager::getFont(0);

    Text enterNameText(font);
    enterNameText.setString("ENTER YOUR NAME: " + inputName + "_");
    enterNameText.setCharacterSize(48);
    enterNameText.setFillColor(Color::White);
    enterNameText.setOutlineColor(Color::Black);
    enterNameText.setOutlineThickness(2);
    enterNameText.setPosition(ScreenConfig::pos(960, 540));
    enterNameText.setOrigin(Vector2f(enterNameText.getLocalBounds().size.x / 2, enterNameText.getLocalBounds().size.y / 2));

    Text bestText(font);
    bestText.setCharacterSize(28);
    bestText.setFillColor(Color::Yellow);
    bestText.setOutlineColor(Color::Black);
    bestText.setOutlineThickness(1);
    bestText.setPosition(ScreenConfig::pos(250, 25));

    Text boostText(font);
    boostText.setCharacterSize(28);
    boostText.setFillColor(Color(255, 215, 0));
    boostText.setOutlineColor(Color::Black);
    boostText.setOutlineThickness(1);
    boostText.setPosition(ScreenConfig::pos(250, 55));

    Text eggRainText(font);
    eggRainText.setCharacterSize(48);
    eggRainText.setFillColor(Color(255, 100, 255));
    eggRainText.setOutlineColor(Color::Black);
    eggRainText.setOutlineThickness(2);
    eggRainText.setPosition(ScreenConfig::pos(960, 150));
    eggRainText.setOrigin(Vector2f(eggRainText.getLocalBounds().size.x / 2, eggRainText.getLocalBounds().size.y / 2));

    Text gameOverText(font);
    gameOverText.setString("GAME OVER! Press R to restart");
    gameOverText.setCharacterSize(72);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setOutlineColor(Color::Black);
    gameOverText.setOutlineThickness(3);

    FloatRect gameOverBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(Vector2f(gameOverBounds.size.x / 2, gameOverBounds.size.y / 2));
    gameOverText.setPosition(ScreenConfig::pos(960, 540));

    Text victoryText(font);
    victoryText.setString("VICTORY! Press R to continue");
    victoryText.setCharacterSize(72);
    victoryText.setFillColor(Color::Yellow);
    victoryText.setOutlineColor(Color::Black);
    victoryText.setOutlineThickness(3);

    FloatRect victoryBounds = victoryText.getLocalBounds();
    victoryText.setOrigin(Vector2f(victoryBounds.size.x / 2, victoryBounds.size.y / 2));
    victoryText.setPosition(ScreenConfig::pos(960, 540));

    bool showVictory = false;
    float victoryTimer = 0.0f;

    // ТЕКСТ ДЛЯ БУСТА "БОКСЁРСКАЯ ПЕРЧАТКА"
    Text boxingGloveText(font);
    boxingGloveText.setCharacterSize(36);
    boxingGloveText.setFillColor(Color(255, 80, 80));
    boxingGloveText.setOutlineColor(Color::Black);
    boxingGloveText.setOutlineThickness(2);
    boxingGloveText.setPosition(ScreenConfig::pos(960, 200));
    boxingGloveText.setOrigin(Vector2f(boxingGloveText.getLocalBounds().size.x / 2, boxingGloveText.getLocalBounds().size.y / 2));

    // ТЕКСТ ДЛЯ БУСТА "БУРГЕР"
    Text burgerText(font);
    burgerText.setCharacterSize(28);
    burgerText.setFillColor(Color(255, 200, 100));
    burgerText.setOutlineColor(Color::Black);
    burgerText.setOutlineThickness(1);
    burgerText.setPosition(ScreenConfig::pos(250, 85));

    // ТЕКСТ ДЛЯ БУСТА "ЗЕЛЬЕ"
    Text potionText(font);
    potionText.setCharacterSize(28);
    potionText.setFillColor(Color(200, 100, 255));
    potionText.setOutlineColor(Color::Black);
    potionText.setOutlineThickness(1);
    potionText.setPosition(ScreenConfig::pos(250, 115));

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

        // ОБНОВЛЕНИЕ БУСТОВ С ПЕРЕДАЧЕЙ СОСТОЯНИЯ БОССА
        powerUpManager.update(time, boss.isActive());

        // ПРОВЕРКА СТОЛКНОВЕНИЙ С БУСТАМИ
        for (auto& powerUp : powerUpManager.getPowerUps()) {
            if (powerUp->collision(player.getBasketBounds())) {
                if (powerUp->getType() == PowerUpType::EggRain) {
                    if (!eggRainActive) {
                        eggRainActive = true;
                        eggRainTimer = EGG_RAIN_DURATION;
                        
                        for (auto& obj : fallingObjects) {
                            if (auto* egg = dynamic_cast<Egg*>(obj.get())) {
                                egg->enableRainMode();
                            }
                        }
                        
                        int newEggsCount = 80;
                        extraEggsCount = newEggsCount;
                        
                        for (int i = 0; i < newEggsCount; i++) {
                            auto newEgg = std::make_unique<Egg>();
                            
                            if (rand() % 100 < 20) {
                                newEgg->setGolden(true);
                            }
                            
                            float x = ScreenConfig::scaleX * (100 + (rand() % 1720));
                            float y = ScreenConfig::scaleY * (-50 - (i * 15));
                            
                            newEgg->forceRainFall(x, y);
                            newEgg->enableRainMode();
                            
                            fallingObjects.push_back(std::move(newEgg));
                        }
                    }
                }
                // БУСТ: БОКСЁРСКАЯ ПЕРЧАТКА
                else if (powerUp->getType() == PowerUpType::BoxingGlove) {
                    player.activateBoxingGlove();
                }
                // БУСТ: БУРГЕР (восстанавливает 25 HP)
                else if (powerUp->getType() == PowerUpType::Burger) {
                    player.heal(25);
                }
                // БУСТ: ЗЕЛЬЕ (восстанавливает 40 HP)
                else if (powerUp->getType() == PowerUpType::Potion) {
                    player.heal(40);
                }
                powerUp->restart();
                break;
            }
        }

        // ДВИЖЕНИЕ И СБОР ЯИЦ
        for (auto& obj : fallingObjects) {
            obj->move(time);
            if (obj->collision(player.getBasketBounds())) {
                if (auto* egg = dynamic_cast<Egg*>(obj.get())) {
                    int points = egg->getGolden() ? 1500 : 500;
                    if (doublePoints) {
                        points *= 2;
                    }
                    scoreCounter.addScore(points);
                    obj->restart();
                }
            }
        }

        // ТАЙМЕР ЯЙЦЕПАДА
        if (eggRainActive) {
            eggRainTimer -= time;
            if (eggRainTimer <= 0.0f) {
                eggRainActive = false;
                
                while (fallingObjects.size() > static_cast<size_t>(count_eggs)) {
                    fallingObjects.pop_back();
                }
                
                for (auto& obj : fallingObjects) {
                    if (auto* egg = dynamic_cast<Egg*>(obj.get())) {
                        egg->disableRainMode();
                        egg->restart();
                    }
                }
                extraEggsCount = 0;
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
                    // УРОН С УЧЁТОМ МНОЖИТЕЛЯ ОТ ПЕРЧАТКИ
                    int damage = 10 * player.getDamageMultiplier();
                    boss.takeDamage(damage);
                    player.getAttack().stop();

                    if (!boss.isAlive()) {
                        boss.reset();
                        bossHealthBar.setActive(false);
                        scoreCounter.addScore(50000);
                        bossDefeated = true;
                        showVictory = true;
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

        if (doublePoints) {
            doublePointsTimer -= time;
            if (doublePointsTimer <= 0.0f) {
                doublePoints = false;
            }
            std::string boostStr = "2X POINTS! " + std::to_string(static_cast<int>(doublePointsTimer)) + "s";
            if (doublePointsTimer < 1.0f) {
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

        // ОТРИСОВКА
        window.clear();
        if (backgroundSprite.getTexture().getNativeHandle() != 0) {
            window.draw(backgroundSprite);
        }
        player.draw(window);
        for (auto& obj : fallingObjects) {
            obj->draw(window);
        }
        scoreCounter.draw(window);
        healthBar.draw(window);
        window.draw(bestText);
        window.draw(boostText);
        window.draw(burgerText);
        window.draw(potionText);
        
        powerUpManager.draw(window);

        if (eggRainActive) {
            if (eggRainTimer < 2.0f && static_cast<int>(eggRainTimer * 10) % 2 == 0) {
                eggRainText.setFillColor(Color::Red);
            } else {
                eggRainText.setFillColor(Color(255, 100, 255));
            }
            
            std::string rainText = "EGG RAIN! " + std::to_string(static_cast<int>(eggRainTimer)) + "s";
            eggRainText.setString(rainText);
            eggRainText.setOrigin(Vector2f(eggRainText.getLocalBounds().size.x / 2, eggRainText.getLocalBounds().size.y / 2));
            
            window.draw(eggRainText);
        }

        // ОТРИСОВКА ТЕКСТА БОКСЁРСКОЙ ПЕРЧАТКИ
        if (player.hasBoxingGlove()) {
            float remaining = player.getBoxingGloveTimer();
            std::string gloveText = "🥊 BOXING GLOVE! x4 DAMAGE 🥊 " + std::to_string(static_cast<int>(remaining)) + "s";
            boxingGloveText.setString(gloveText);
            
            if (remaining < 3.0f && static_cast<int>(remaining * 10) % 2 == 0) {
                boxingGloveText.setFillColor(Color::Red);
            } else {
                boxingGloveText.setFillColor(Color(255, 100, 100));
            }
            
            boxingGloveText.setOrigin(Vector2f(boxingGloveText.getLocalBounds().size.x / 2, boxingGloveText.getLocalBounds().size.y / 2));
            window.draw(boxingGloveText);
        }

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

            window.draw(gameOverText);

            if (Keyboard::isKeyPressed(Keyboard::Key::R)) {
                restartGame(player, scoreCounter, boss, bossHealthBar, bossDefeated,
                    dialogShown, waitingForChoice, doublePoints, doublePointsTimer,
                    scoreSaved, playerChoseStop, eggRainActive, eggRainTimer,
                    powerUpManager, extraEggsCount, fallingObjects, count_eggs, showVictory);
            }
        }

        // БЛОК ПОБЕДЫ
        if (showVictory) {
            window.draw(victoryText);

            if (Keyboard::isKeyPressed(Keyboard::Key::R)) {
                restartGame(player, scoreCounter, boss, bossHealthBar, bossDefeated,
                    dialogShown, waitingForChoice, doublePoints, doublePointsTimer,
                    scoreSaved, playerChoseStop, eggRainActive, eggRainTimer,
                    powerUpManager, extraEggsCount, fallingObjects, count_eggs, showVictory);
            }
        }

        bossDialog.draw(window);
        window.display();
    }

    return 0;
}
