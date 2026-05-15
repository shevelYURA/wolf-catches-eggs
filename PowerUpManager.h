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
    PowerUpManager() : spawnTimer(0), normalSpawnInterval(25.0f), bossSpawnInterval(15.0f) {}

    void update(float time, bool isBossActive) {
        spawnTimer += time;
        
        float currentInterval = isBossActive ? bossSpawnInterval : normalSpawnInterval;
        
        if (spawnTimer >= currentInterval) {
            spawnTimer = 0;
            
            // Устанавливаем следующий интервал
            if (isBossActive) {
                bossSpawnInterval = 12.0f + (rand() % 8);  // ← 12-20 секунд
            } else {
                normalSpawnInterval = 20.0f + (rand() % 10);
            }
            
            // ВЫБОР ТИПА БУСТА
            PowerUpType type;
            
            if (isBossActive) {
                // 50% шанс перчатки, 50% шанс других бустов
                int randomType = rand() % 2;  // 0 или 1
                if (randomType == 0) {
                    type = PowerUpType::BoxingGlove;  // ← ПЕРЧАТКА
                } else {
                    int otherType = rand() % 2;
                    type = (otherType == 0) ? PowerUpType::EggRain : PowerUpType::DoublePoints;
                }
            } else {
                // Когда босса нет: только яйцепад и двойные очки
                int randomType = rand() % 2;
                type = (randomType == 0) ? PowerUpType::EggRain : PowerUpType::DoublePoints;
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
        normalSpawnInterval = 25.0f;
        bossSpawnInterval = 15.0f;
    }
};
