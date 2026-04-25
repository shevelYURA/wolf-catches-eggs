#pragma once
#include "Eggs.h"
#include <vector>
#include <cmath>
#include <algorithm>

class EggFallBoost {
private:
    static inline bool isActive = false;
    static inline float boostTimer = 0.f;
    static inline std::vector<Egg*> boostedEggs;
    static inline std::vector<bool> eggActive;  // активны ли яйца

public:
    static void activate(float duration = 8.0f) {
        if (isActive) return;
        
        // Очищаем старые яйца
        deactivate();
        
        isActive = true;
        boostTimer = duration;
        createSnakeEggs();
    }
    
    static void createSnakeEggs() {
        const int eggCount = 45;
        const float startY = 50.f;
        const float amplitude = 250.f;
        const float frequency = 0.04f;
        const float spacingY = 42.f;
        
        for (int i = 0; i < eggCount; i++) {
            float t = i * frequency;
            float centerX = 960.f;
            float x = centerX + sin(t) * amplitude;
            float y = startY + i * spacingY;
            
            Egg* egg = new Egg();
            egg->setPosition(sf::Vector2f(x, y));
            boostedEggs.push_back(egg);
            eggActive.push_back(true);  // все яйца активны
        }
    }
    
    static void update(float deltaTime) {
        if (!isActive) return;
        
        boostTimer -= deltaTime;
        if (boostTimer <= 0) {
            deactivate();
            return;
        }
        
        float speed = 250.f;
        
        for (int i = 0; i < (int)boostedEggs.size(); i++) {
            if (!eggActive[i]) continue;  // пропускаем неактивные
            
            Egg* egg = boostedEggs[i];
            sf::Vector2f pos = egg->getPosition();
            pos.y += speed * deltaTime;
            egg->setPosition(pos);
            
            // Если упало за экран - делаем неактивным (НЕ удаляем!)
            if (pos.y > 1080) {
                eggActive[i] = false;
            }
        }
    }
    
    static void deactivate() {
        isActive = false;
        
        // Удаляем все яйца
        for (auto* egg : boostedEggs) {
            delete egg;
        }
        boostedEggs.clear();
        eggActive.clear();
    }
    
    static bool isBoostActive() {
        return isActive;
    }
    
    static std::vector<Egg*>& getEggs() {
        return boostedEggs;
    }
    
    static bool isEggAlive(int index) {
        if (index >= 0 && index < (int)eggActive.size()) {
            return eggActive[index];
        }
        return false;
    }
};
