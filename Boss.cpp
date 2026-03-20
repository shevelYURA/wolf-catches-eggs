#include "Boss.h"
#include <cmath>
#include <cstdlib>

Boss::Boss() : health(100), active(false), moveSpeed(150.0f), directionChangeTimer(0)
{
    if (!texBoss.loadFromFile("image/bossTexture.png")) {
        // Если нет текстуры босса, создадим заливку цветом
        body.setFillColor(Color(50, 50, 150));
    }
    else {
        body.setTexture(&texBoss);
    }

    body.setSize(Vector2f(120, 120));
    body.setOrigin(Vector2f(60, 60));
    body.setPosition(Vector2f(960, 200));

    // Глаза
    eyeLeft.setRadius(15);
    eyeLeft.setFillColor(Color::White);
    eyeLeft.setOrigin(Vector2f(15, 15));

    eyeRight.setRadius(15);
    eyeRight.setFillColor(Color::White);
    eyeRight.setOrigin(Vector2f(15, 15));

    pupilLeft.setRadius(7);
    pupilLeft.setFillColor(Color::Black);
    pupilLeft.setOrigin(Vector2f(7, 7));

    pupilRight.setRadius(7);
    pupilRight.setFillColor(Color::Black);
    pupilRight.setOrigin(Vector2f(7, 7));

    // Начальное направление движения
    direction = Vector2f(1, 0);
}

void Boss::update(float time, const RenderWindow& window, const Vector2f& playerPosition)
{
    if (!active) return;

    updateMovement(time);
    updateEyes(playerPosition);
    checkBounds();
}

void Boss::updateMovement(float time)
{
    directionChangeTimer += time;

    // Меняем направление каждые 2-3 секунды
    if (directionChangeTimer >= 2.0f + (rand() % 2)) {
        directionChangeTimer = 0;
        float angle = static_cast<float>(rand() % 360) * 3.14159f / 180.0f;
        direction = Vector2f(std::cos(angle), std::sin(angle));
    }

    Vector2f newPos = body.getPosition() + direction * moveSpeed * time;
    body.setPosition(newPos);
}

void Boss::updateEyes(const Vector2f& playerPosition)
{
    Vector2f bossPos = body.getPosition();
    Vector2f toPlayer = playerPosition - bossPos;
    float length = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

    if (length > 0) {
        toPlayer /= length;

        // Зрачки смотрят на игрока
        float eyeOffsetX = toPlayer.x * 8;
        float eyeOffsetY = toPlayer.y * 8;

        pupilLeft.setPosition(Vector2f(bossPos.x - 35 + eyeOffsetX, bossPos.y - 25 + eyeOffsetY));
        pupilRight.setPosition(Vector2f(bossPos.x + 35 + eyeOffsetX, bossPos.y - 25 + eyeOffsetY));

        // Белки глаз
        eyeLeft.setPosition(Vector2f(bossPos.x - 35, bossPos.y - 25));
        eyeRight.setPosition(Vector2f(bossPos.x + 35, bossPos.y - 25));
    }
    else {
        // Если игрок на той же позиции, просто центрируем зрачки
        pupilLeft.setPosition(Vector2f(bossPos.x - 35, bossPos.y - 25));
        pupilRight.setPosition(Vector2f(bossPos.x + 35, bossPos.y - 25));
        eyeLeft.setPosition(Vector2f(bossPos.x - 35, bossPos.y - 25));
        eyeRight.setPosition(Vector2f(bossPos.x + 35, bossPos.y - 25));
    }
}

void Boss::checkBounds()
{
    Vector2f pos = body.getPosition();

    if (pos.x < 100) pos.x = 100;
    if (pos.x > 1820) pos.x = 1820;
    if (pos.y < 50) pos.y = 50;
    if (pos.y > 800) pos.y = 800;

    body.setPosition(pos);
}

void Boss::draw(RenderWindow& window)
{
    if (!active) return;

    window.draw(body);
    window.draw(eyeLeft);
    window.draw(eyeRight);
    window.draw(pupilLeft);
    window.draw(pupilRight);
}

void Boss::takeDamage(int damage)
{
    if (!active) return;

    health -= damage;
    if (health < 0) health = 0;
    updateColor();
}

bool Boss::isAlive() const
{
    return active && health > 0;
}

int Boss::getHealth() const
{
    return health;
}

FloatRect Boss::getBounds() const
{
    return body.getGlobalBounds();
}

bool Boss::isActive() const
{
    return active;
}

void Boss::activate()
{
    active = true;
    health = 100;
    body.setPosition(Vector2f(960, 200));
    updateColor();
}

void Boss::reset()
{
    active = false;
    health = 100;
}

void Boss::updateColor()
{
    // Изменяем цвет в зависимости от здоровья
    float healthPercent = static_cast<float>(health) / 100.0f;

    if (healthPercent > 0.6f) {
        body.setFillColor(Color(150, 50, 150));
    }
    else if (healthPercent > 0.3f) {
        body.setFillColor(Color(200, 100, 50));
    }
    else {
        body.setFillColor(Color(200, 50, 50));
    }
}