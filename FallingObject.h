#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class FallingObject {
    void init(int resourceId, const Vector2f& size);
public:
    FallingObject(int resourceId, const Vector2f& size);
    virtual ~FallingObject() = default;

    virtual void move(float time);
    virtual void draw(RenderWindow& window);
    virtual bool collision(FloatRect object);
    virtual void restart();

    FloatRect getBounds() const;
    bool isFalling() const;
    void setColor(const sf::Color& color);
    
    // НОВЫЕ МЕТОДЫ ДЛЯ БУСТА
    void setPosition(const Vector2f& pos) { shape.setPosition(pos); }
    Vector2f getPosition() const { return shape.getPosition(); }
    
    // РЕЖИМ ЯЙЦЕПАДА
    void enableEggRainMode() { eggRainMode = true; waitTimer = waitTime; currentState = falling; }
    void disableEggRainMode() { eggRainMode = false; }
    bool isEggRainMode() const { return eggRainMode; }
    
    // ПРИНУДИТЕЛЬНОЕ ПАДЕНИЕ (для новых яиц)
    void forceFall(const Vector2f& pos) {
        shape.setPosition(pos);
        currentState = falling;
        eggRainMode = true;
    }

protected:
    RectangleShape shape;
    Texture texture;

    enum State {
        waiting,
        falling
    };

    State currentState;
    float speed;
    float waitTimer;
    float waitTime;
    
    bool eggRainMode = false;
};
