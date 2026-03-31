#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "BossBullet.h"
using namespace sf;

class BossAttack {
    enum class AttackType {
        HomingSalvo,
        LeftBombard,
        RightBombard,
        RandomRain
    };

    std::vector<std::unique_ptr<BossBullet>> bullets;

    AttackType getRandomAttackType();

    float attackTimer;
    float attackInterval;
    bool isActive_;

    float salvoTimer;
    int salvoCount;
    int salvoMax;
    bool isPerformingSalvo;

    void performHomingSalvo(const Vector2f& bossPos, const Vector2f& playerPos);
    void performLeftBombard(const Vector2f& bossPos);
    void performRightBombard(const Vector2f& bossPos);
    void performRandomRain();

    void cleanupInactiveBullets();

public:
    BossAttack();

    void update(float time, const Vector2f& bossPos, const Vector2f& playerPos);
    void draw(RenderWindow& window);

    void reset();
    void activate();
    void deactivate();
    bool isActive() const;

    std::vector<std::unique_ptr<BossBullet>>& getBullets() { return bullets; }
};