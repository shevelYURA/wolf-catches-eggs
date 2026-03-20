#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Player
{
public:
    Player();

    void update(float time);
    void draw(RenderWindow& window);
    FloatRect getBasketBounds() const;

    int getHealth() const;
    void takeDamage(int damage);
    bool isAlive() const;
    void reset();

private:
    RectangleShape wolf;
    RectangleShape basket;
    Texture texWolf;
    Texture texBasket;

    int health;
    void checkHealth();
};