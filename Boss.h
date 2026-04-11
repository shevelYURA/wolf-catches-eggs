#pragma once
#include <SFML/Graphics.hpp>
#include "PlayersAttack.h"
#include "BossAttack.h"
#include "ResourceManager.h"
using namespace sf;

class Boss
{
    RectangleShape body;
    Texture texBoss;
    CircleShape eyeLeft;
    CircleShape eyeRight;
    CircleShape pupilLeft;
    CircleShape pupilRight;

    int health;
    bool active;
    float moveSpeed;
    Vector2f direction;
    float directionChangeTimer;

    BossAttack attackSystem;

    void updateEyes(const Vector2f& playerPosition);
    void updateMovement(float time);
    void checkBounds();
    void updateColor();

public:
    Boss();

    void update(float time, const RenderWindow& window, const Vector2f& playerPosition, const FloatRect& playerBounds);
    void draw(RenderWindow& window);
    void takeDamage(int damage);

    bool isAlive() const;
    int getHealth() const;
    FloatRect getBounds() const;
    bool isActive() const;
    void activate();
    void reset();

    BossAttack& getAttackSystem() { return attackSystem; }
};

