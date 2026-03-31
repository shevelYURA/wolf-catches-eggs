#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class BossBullet {
public:
    enum class Type {
        Homing,
        Vertical,
        HoverLeft,
        HoverRight
    };

    BossBullet(Type type, const Vector2f& startPos, const Vector2f& targetPos = Vector2f(0, 0), float individualDelay = 0.0f);

    void update(float time, const Vector2f& playerPos = Vector2f(0, 0));
    void draw(RenderWindow& window);
    bool isActive() const;
    FloatRect getBounds() const;
    bool checkPlayerCollision(const FloatRect& playerBounds);
    void deactivate();

private:
    RectangleShape shape;
    Texture texture;

    Type type;
    Vector2f position;
    Vector2f velocity;
    Vector2f targetPosition;
    Vector2f hoverStartPos;
    float hoverTimer;
    float hoverDuration;
    float individualDelay;      // Индивидуальная задержка перед началом зависания
    float delayTimer;           // Таймер задержки
    bool isHovering;
    bool isDelaying;            // Состояние задержки

    bool active;
    float speed;

    void initShape();
    void updateHoming(float time);
    void updateVertical(float time);
    void updateHover(float time);
};