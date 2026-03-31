#include "BossBullet.h"
#include <cmath>
#include <cstdlib>

BossBullet::BossBullet(Type type, const Vector2f& startPos, const Vector2f& targetPos, float individualDelay)
    : type(type), position(startPos), targetPosition(targetPos), active(true), speed(300.0f),
    hoverTimer(0), hoverDuration(0.8f), isHovering(false), individualDelay(individualDelay),
    delayTimer(0), isDelaying(individualDelay > 0)
{
    initShape();

    switch (type) {
    case Type::Homing:
        speed = 300.0f;
        {
            Vector2f direction = targetPosition - startPos;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length > 0) {
                direction /= length;
                velocity = direction * speed;
            }
            else {
                velocity = Vector2f(0, 1) * speed;
            }
        }
        break;

    case Type::Vertical:
        velocity = Vector2f(0, speed);
        break;

    case Type::HoverLeft:
    case Type::HoverRight:
        hoverStartPos = startPos;
        // Если есть задержка, начинаем с состояния задержки
        if (isDelaying) {
            velocity = Vector2f(0, 0);
            isHovering = false;
        }
        else {
            isHovering = true;
            velocity = Vector2f(0, 0);
        }
        break;
    }

    shape.setPosition(position);
}

void BossBullet::initShape()
{
    if (!texture.loadFromFile("image/bombTexture.png")) {
        shape.setSize(Vector2f(35, 35));
        shape.setFillColor(Color(255, 100, 100));
    }
    else {
        shape.setSize(Vector2f(35, 35));
        shape.setTexture(&texture);
    }
    shape.setOrigin(Vector2f(17.5f, 17.5f));
}

void BossBullet::update(float time, const Vector2f& playerPos)
{
    if (!active) return;

    // Обработка задержки перед началом
    if (isDelaying) {
        delayTimer += time;
        if (delayTimer >= individualDelay) {
            isDelaying = false;
            isHovering = true;  // После задержки начинаем зависание
            hoverTimer = 0;
        }
        // Во время задержки бомба не двигается
        shape.setPosition(position);
        return;
    }

    switch (type) {
    case Type::Homing:
        updateHoming(time);
        break;
    case Type::Vertical:
        updateVertical(time);
        break;
    case Type::HoverLeft:
    case Type::HoverRight:
        updateHover(time);
        break;
    }

    if (position.y > 1200 || position.y < -100 || position.x < -100 || position.x > 2020) {
        active = false;
    }

    shape.setPosition(position);
}

void BossBullet::updateHoming(float time)
{
    position += velocity * time;
}

void BossBullet::updateVertical(float time)
{
    position += velocity * time;
}

void BossBullet::updateHover(float time)
{
    if (isHovering) {
        hoverTimer += time;
        if (hoverTimer >= hoverDuration) {
            isHovering = false;
            velocity = Vector2f(0, speed);
        }
    }
    else {
        position += velocity * time;
    }
}

void BossBullet::draw(RenderWindow& window)
{
    if (active) {
        window.draw(shape);
    }
}

bool BossBullet::isActive() const
{
    return active;
}

FloatRect BossBullet::getBounds() const
{
    return shape.getGlobalBounds();
}

bool BossBullet::checkPlayerCollision(const FloatRect& playerBounds)
{
    if (!active) return false;

    if (getBounds().findIntersection(playerBounds).has_value()) {
        active = false;
        return true;
    }
    return false;
}

void BossBullet::deactivate()
{
    active = false;
}