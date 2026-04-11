#pragma once
#include <SFML/Graphics.hpp>
#include "PlayersAttack.h"
#include "ResourceManager.h"
using namespace sf;

class Player
{
    RectangleShape wolf;
    RectangleShape basket;
    Texture texWolf;
    Texture texBasket;

    int health;
    void checkHealth();

    PlayersAttack attack;
    bool attackKeyPressed;
    void handleAttack(const sf::RenderWindow& window);

public:
    Player();

    void update(float time, const RenderWindow& window);
    void draw(RenderWindow& window);
    FloatRect getBasketBounds() const;
    FloatRect getBounds() const;
    Vector2f getBottomCenter() const;

    int getHealth() const;
    void takeDamage(int damage);
    bool isAlive() const;
    void reset();

    PlayersAttack& getAttack() { return attack; }

    Vector2f getPosition() const { return wolf.getPosition(); }
};