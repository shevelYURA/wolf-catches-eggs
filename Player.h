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

private:
    RectangleShape wolf;
    RectangleShape basket;
    Texture texWolf;
    Texture texBasket;
};