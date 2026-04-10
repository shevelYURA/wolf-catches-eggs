#include "FallingObject.h"

FallingObject::FallingObject(const std::string& texturePath, const Vector2f& size) {
    init(texturePath, size);
    restart();
}

void FallingObject::init(const std::string& texturePath, const Vector2f& size) {
    if (!texture.loadFromFile(texturePath)) {
        throw;
    }

    shape.setSize(size);
    shape.setTexture(&texture);
    speed = 70.0f;
}

void FallingObject::move(float time) {
    switch (currentState) {
    case waiting:
        waitTimer += time;
        if (waitTimer >= waitTime) {
            currentState = falling;
            float x = static_cast<float>(rand() % 1820 + 50);
            shape.setPosition(Vector2f{ x, 0.0f });
        }
        break;

    case falling:
        shape.move({ 0.0f, speed * time * 1.5f });
        break;
    }

    if (shape.getPosition().y > 1080) {
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
    waitTime = (rand() % 5) + 1;
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
