#include "BossAttack.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <functional>

BossAttack::BossAttack()
    : attackTimer(0), attackInterval(4.0f), isActive_(false),
    salvoTimer(0), salvoCount(0), salvoMax(0), isPerformingSalvo(false)
{
}

void BossAttack::update(float time, const Vector2f& bossPos, const Vector2f& playerPos)
{
    if (!isActive_) return;

    cleanupInactiveBullets();

    for (auto& bullet : bullets) {
        bullet->update(time, playerPos);
    }

    if (isPerformingSalvo) {
        salvoTimer += time;
        if (salvoTimer >= 1.5f) {
            salvoTimer = 0;
            salvoCount++;

            if (salvoCount < salvoMax) {
                Vector2f startPos = bossPos;
                bullets.push_back(std::make_unique<BossBullet>(
                    BossBullet::Type::Homing, startPos, playerPos));
            }
            else {
                isPerformingSalvo = false;
                attackTimer = 0;
            }
        }
        return;
    }

    attackTimer += time;
    if (attackTimer >= attackInterval) {
        attackTimer = 0;

        AttackType type = getRandomAttackType();

        switch (type) {
        case AttackType::HomingSalvo:
            performHomingSalvo(bossPos, playerPos);
            break;
        case AttackType::LeftBombard:
            performLeftBombard(bossPos);
            break;
        case AttackType::RightBombard:
            performRightBombard(bossPos);
            break;
        case AttackType::RandomRain:
            performRandomRain();
            break;
        }
    }
}

BossAttack::AttackType BossAttack::getRandomAttackType()
{
    int random = rand() % 4;
    return static_cast<AttackType>(random);
}

void BossAttack::performHomingSalvo(const Vector2f& bossPos, const Vector2f& playerPos)
{
    salvoMax = 6;
    salvoCount = 0;
    salvoTimer = 0;
    isPerformingSalvo = true;
    bullets.push_back(std::make_unique<BossBullet>(
        BossBullet::Type::Homing, bossPos, playerPos));
    attackTimer = attackInterval * 0.5f;
}

void BossAttack::performLeftBombard(const Vector2f& bossPos)
{
    float startY = 440;
    int bombCount = 20;
    float safeZoneStart = 1400.0f;
    std::vector<float> xPositions;
    for (int i = 0; i < bombCount; i++) {
        float startX = (safeZoneStart / bombCount) * i;
        xPositions.push_back(startX);
    }

    std::sort(xPositions.begin(), xPositions.end());

    for (size_t i = 0; i < xPositions.size(); i++) {
        float delay = i * 0.15f;
        auto bullet = std::make_unique<BossBullet>(
            BossBullet::Type::HoverLeft,
            Vector2f(xPositions[i], startY),
            Vector2f(0, 0),
            delay);
        bullets.push_back(std::move(bullet));
    }
}

void BossAttack::performRightBombard(const Vector2f& bossPos)
{
    float startY = 440;
    int bombCount = 20;
    float safeZoneEnd = 520.0f;
    float rightEdge = 1920.0f;
    float range = rightEdge - safeZoneEnd;

    std::vector<float> xPositions;
    for (int i = 0; i < bombCount; i++) {
        float startX = safeZoneEnd + (range / bombCount) * i;
        xPositions.push_back(startX);
    }

    std::sort(xPositions.begin(), xPositions.end(), std::greater<float>());

    for (size_t i = 0; i < xPositions.size(); i++) {
        float delay = i * 0.15f;
        auto bullet = std::make_unique<BossBullet>(
            BossBullet::Type::HoverRight,
            Vector2f(xPositions[i], startY),
            Vector2f(0, 0),
            delay);
        bullets.push_back(std::move(bullet));
    }
}

void BossAttack::performRandomRain()
{
    int bombCount = 13;

    for (int i = 0; i < bombCount; i++) {
        float startX = 100.0f + (rand() % 1720);
        float startY = -50.0f;
        auto bullet = std::make_unique<BossBullet>(
            BossBullet::Type::Vertical, Vector2f(startX, startY));
        bullets.push_back(std::move(bullet));
    }
}

void BossAttack::cleanupInactiveBullets()
{
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const std::unique_ptr<BossBullet>& bullet) {
                return !bullet->isActive();
            }),
        bullets.end()
    );
}

void BossAttack::draw(RenderWindow& window)
{
    for (auto& bullet : bullets) {
        bullet->draw(window);
    }
}

void BossAttack::reset()
{
    bullets.clear();
    isActive_ = false;
    isPerformingSalvo = false;
    attackTimer = 0;
    salvoCount = 0;
}

void BossAttack::activate()
{
    reset();
    isActive_ = true;
}

void BossAttack::deactivate()
{
    reset();
}

bool BossAttack::isActive() const
{
    return isActive_;
}