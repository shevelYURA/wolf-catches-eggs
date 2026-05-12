#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "PowerUp.h"
#include "screenConfig.h"

class PowerUpManager {
private:
    std::vector<std::unique_ptr<PowerUp>> powerUps;
    float spawnTimer;
    float spawnInterval;

public:
    PowerUpManager() : spawnTimer(0), spawnInterval(12.0f) {}

    void update(float time) {
        // Спавн новых бустов
        spawnTimer += time;
        if (spawnTimer >= spawnInterval) {
            spawnTimer = 0;
            spawnInterval = 8.0f + (rand() % 8);
            
            auto powerUp = std::make_unique<PowerUp>(PowerUpType::EggRain);
            float randomX = static_cast<float>(rand() % static_cast<int>(ScreenConfig::scaleX * 1800) + 50);
            // Устанавливаем позицию вручную через setPosition (добавим в FallingObject)
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
        spawnInterval = 12.0f;
    }
};
