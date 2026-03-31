#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;

class HealthBar
{
    RectangleShape healthBarBackground;
    RectangleShape healthBarForeground;
    Text* healthText;
    Font font;
    void updatePosition();
public:
    HealthBar();
    ~HealthBar();

    void update(int currentHealth, int maxHealth = 100);
    void draw(sf::RenderWindow& window);
};