#pragma once
#include "Eggs.h"
#include <vector>
#include <cmath>
#include <algorithm>

class EggFallBoost {
private:
    static bool isActive;
    static float boostTimer;
    static std::vector<Egg*> boostedEggs;

public:
    static void activate(float duration = 8.0f) {
        if (isActive) return;
        
        isActive = true;
        boostTimer = duration;
        
        createSnakeEggs();
    }
    
    static void createSnakeEggs() {
        const int eggCount = 65;
        const float startY = 50.f;
        const float amplitude = 300.f;
        const float frequency = 0.04f;
        const float spacingY = 38.f;
        
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
    
    static void update(float deltaTime, std::vector<Egg*>& mainEggs) {
        if (!isActive) return;
        
        boostTimer -= deltaTime;
        if (boostTimer <= 0) {
            deactivate(mainEggs);
            return;
        }
        
        for (auto it = boostedEggs.begin(); it != boostedEggs.end();) {
            Egg* egg = *it;
            float speed = 200.f;
            egg->move(sf::Vector2f(0.f, speed * deltaTime));
            
            if (egg->getPosition().y > 1080) {
                delete egg;
                it = boostedEggs.erase(it);
            } else {
                mainEggs.push_back(egg);
                ++it;
            }
        }
    }
    
    static void deactivate(std::vector<Egg*>& mainEggs) {
        isActive = false;
        
        for (auto* egg : boostedEggs) {
            auto it = std::find(mainEggs.begin(), mainEggs.end(), egg);
            if (it != mainEggs.end()) {
                mainEggs.erase(it);
            }
            delete egg;
        }
        boostedEggs.clear();
    }
    
    static bool isBoostActive() {
        return isActive;
    }
};

bool EggFallBoost::isActive = false;
float EggFallBoost::boostTimer = 0.f;
std::vector<Egg*> EggFallBoost::boostedEggs;
