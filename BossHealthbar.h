#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;

class BossHealthBar
{
public:
    BossHealthBar();
    ~BossHealthBar();

    void update(int currentHealth, int maxHealth = 100);
    void draw(sf::RenderWindow& window);
    void setActive(bool active);
    bool isActive() const;

private:
    RectangleShape healthBarBackground;
    RectangleShape healthBarForeground;
    Text* healthText;
    Font font;
    bool active;

    void updatePosition();
};
