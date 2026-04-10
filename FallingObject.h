#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class FallingObject {
    void init(const std::string& texturePath, const Vector2f& size);
public:
    FallingObject(const std::string& texturePath, const Vector2f& size);
    virtual ~FallingObject() = default;

    virtual void move(float time);
    virtual void draw(RenderWindow& window);
    virtual bool collision(FloatRect object);
    virtual void restart();

    FloatRect getBounds() const;
    bool isFalling() const;
void setColor(const sf::Color& color);

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
};
