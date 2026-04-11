#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;

class BossHealthBar
{
    RectangleShape healthBarBackground;
    RectangleShape healthBarForeground;
    Text* healthText;
    Font font;
    bool active;

    void updatePosition();

public:
    BossHealthBar();
    ~BossHealthBar();

    void update(int currentHealth, int maxHealth = 1000);
    void draw(sf::RenderWindow& window);
    void setActive(bool active);
    bool isActive() const;
};
