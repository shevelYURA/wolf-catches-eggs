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

public:
    static void activate(float duration = 8.0f) {
        if (isActive) return;
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
        }
    }
    
    static void update(float deltaTime, std::vector<Egg*>& boostEggs) {
        if (!isActive) return;
        
        boostTimer -= deltaTime;
        if (boostTimer <= 0) {
            deactivate(boostEggs);
            return;
        }
        
        // Обновляем позиции яиц
        for (int i = 0; i < (int)boostedEggs.size(); i++) {
            Egg* egg = boostedEggs[i];
            if (!egg) continue;
            
            float speed = 250.f;
            sf::Vector2f pos = egg->getPosition();
            pos.y += speed * deltaTime;
            egg->setPosition(pos);
            
            // Если яйцо упало за экран - удаляем
            if (egg->getPosition().y > 1080) {
                delete egg;
                boostedEggs.erase(boostedEggs.begin() + i);
                i--;
            }
        }
        
        // Обновляем внешний вектор для отрисовки
        boostEggs.clear();
        for (auto* egg : boostedEggs) {
            boostEggs.push_back(egg);
        }
    }
    
    static void deactivate(std::vector<Egg*>& boostEggs) {
        isActive = false;
        
        // Удаляем все яйца буста
        for (auto* egg : boostedEggs) {
            delete egg;
        }
        boostedEggs.clear();
        boostEggs.clear();
    }
    
    static bool isBoostActive() {
        return isActive;
    }
};
