#include "FallingObject.h"
#include "ResourceManager.h"
#include "screenConfig.h"

FallingObject::FallingObject(int resourceId, const Vector2f& size) {
    texture = ResourceManager::getTexture(resourceId);
    shape.setSize(size);
    shape.setTexture(&texture);
    speed = 70.0f;
    restart();
}

void FallingObject::init(int resourceId, const Vector2f& size) {
    texture = ResourceManager::getTexture(resourceId);
    shape.setSize(size);
    shape.setTexture(&texture);
    shape.setFillColor(sf::Color::White);
    speed = 70.0f;
}

void FallingObject::move(float time) {
    // РЕЖИМ ЯЙЦЕПАДА - падают строго вниз быстро
    if (eggRainMode) {
        shape.move({0.0f, speed * time * 2.5f});  // Ускоренное падение
        if (shape.getPosition().y > ScreenConfig::scaleY * 1200) {
            // Убираем за экран
            shape.setPosition(Vector2f{-100, -100});
            currentState = waiting;
        }
        return;
    }
    
    // ОБЫЧНЫЙ РЕЖИМ
    switch (currentState) {
    case waiting:
        waitTimer += time;
        if (waitTimer >= waitTime) {
            currentState = falling;
            float x = static_cast<float>(rand() % static_cast<int>(ScreenConfig::scaleX * 1820) + static_cast<int>(ScreenConfig::scaleX * 50));
            shape.setPosition(Vector2f{ x, 0.0f });
        }
        break;

    case falling:
        shape.move({ 0.0f, speed * time * 1.5f });
        break;
    }

    if (shape.getPosition().y > ScreenConfig::scaleY * 1080) {
        restart();
    }
}

void FallingObject::draw(RenderWindow& window) {
    if (currentState == falling) {
        window.draw(shape);
    }
}

bool FallingObject::collision(FloatRect object) {
    if (currentState == falling) {
        return shape.getGlobalBounds().findIntersection(object).has_value();
    }
    return false;
}

void FallingObject::restart() {
    waitTime = static_cast<float>((rand() % 5) + 1);
    waitTimer = 0;
    currentState = waiting;
    shape.setPosition(Vector2f{ -100, -100 });
}

FloatRect FallingObject::getBounds() const {
    return shape.getGlobalBounds();
}

bool FallingObject::isFalling() const {
    return currentState == falling;
}

void FallingObject::setColor(const sf::Color& color) {
    shape.setFillColor(color);
}
