#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "PowerUp.h"
#include "screenConfig.h"

using namespace sf;

class PowerUpManager {
private:
    std::vector<std::unique_ptr<PowerUp>> powerUps;
    float spawnTimer;
    float normalSpawnInterval;
    float bossSpawnInterval;

public:
    PowerUpManager() : spawnTimer(0), normalSpawnInterval(20.0f), bossSpawnInterval(15.0f) {}

    void update(float time, bool isBossActive) {
        spawnTimer += time;
        
        float currentInterval = isBossActive ? bossSpawnInterval : normalSpawnInterval;
        
        if (spawnTimer >= currentInterval) {
            spawnTimer = 0;
            
            // Устанавливаем следующий интервал
            if (isBossActive) {
                bossSpawnInterval = 12.0f + (rand() % 8);
            } else {
                normalSpawnInterval = 20.0f + (rand() % 10);
            }
            
            // ВЫБОР ТИПА БУСТА
            PowerUpType type;
            
            if (isBossActive) {
                // КОГДА БОСС ЕСТЬ (падают все бусты)
                // 20% Перчатка, 20% Яйцепад, 20% Двойные очки, 30% Бургер, 10% Зелье
                int randomType = rand() % 10;
                if (randomType < 2) {
                    type = PowerUpType::BoxingGlove;      // 20%
                } else if (randomType < 4) {
                    type = PowerUpType::EggRain;          // 20%
                } else if (randomType < 6) {
                    type = PowerUpType::DoublePoints;     // 20%
                } else if (randomType < 9) {
                    type = PowerUpType::Burger;           // 30%
                } else {
                    type = PowerUpType::Potion;           // 10%
                }
            } else {
                // КОГДА БОССА НЕТ (ТОЛЬКО обычные бусты, БЕЗ бургера и зелья)
                // 50% Яйцепад, 50% Двойные очки
                int randomType = rand() % 2;
                if (randomType == 0) {
                    type = PowerUpType::EggRain;
                } else {
                    type = PowerUpType::DoublePoints;
                }
            }
            
            auto powerUp = std::make_unique<PowerUp>(type);
            float randomX = static_cast<float>(rand() % static_cast<int>(ScreenConfig::scaleX * 1800) + 50);
            powerUp->setPosition(Vector2f(randomX, -50));
            powerUps.push_back(std::move(powerUp));
        }

        // Движение всех бустов
        for (auto& powerUp : powerUps) {
            powerUp->move(time);
        }

        // Удаление упавших
        powerUps.erase(std::remove_if(powerUps.begin(), powerUps.end(),
            [](const std::unique_ptr<PowerUp>& p) {
                return p->getBounds().position.y > ScreenConfig::scaleY * 1100;
            }), powerUps.end());
    }

    void draw(RenderWindow& window) {
        for (auto& powerUp : powerUps) {
            powerUp->draw(window);
        }
    }

    std::vector<std::unique_ptr<PowerUp>>& getPowerUps() { return powerUps; }

    void reset() {
        powerUps.clear();
        spawnTimer = 0;
        normalSpawnInterval = 20.0f;
        bossSpawnInterval = 15.0f;
    }
};
