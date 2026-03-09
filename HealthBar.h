#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;

class HealthBar
{
public:
    HealthBar();
    ~HealthBar();

    void update(int currentHealth, int maxHealth = 100);
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape healthBarBackground;
    sf::RectangleShape healthBarForeground;
    sf::Text* healthText;
    sf::Font font;

    void updatePosition();
};