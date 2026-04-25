#include "HealthBar.h"
#include "ResourceManager.h"
#include "ScreenConfig.h"

HealthBar::HealthBar() : healthText(nullptr)
{
    font = ResourceManager::getFont(0);

    healthText = new Text(font);

    // Настройка фона полоски здоровья
    healthBarBackground.setSize(Vector2f(400, 30));
    healthBarBackground.setFillColor(Color(100, 100, 100));
    healthBarBackground.setOutlineColor(Color::Black);
    healthBarBackground.setOutlineThickness(2);

    // Настройка полоски здоровья
    healthBarForeground.setSize(Vector2f(400, 30));
    healthBarForeground.setFillColor(Color::Green);

    // Настройка текста здоровья
    healthText->setString("Health: 100/100");
    healthText->setCharacterSize(36);
    healthText->setFillColor(Color::White);
    healthText->setOutlineColor(Color::Black);
    healthText->setOutlineThickness(1);

    updatePosition();
}

HealthBar::~HealthBar()
{
    delete healthText;
}

void HealthBar::update(int currentHealth, int maxHealth)
{
    float healthPercent = static_cast<float>(currentHealth) / maxHealth;
    healthBarForeground.setSize(Vector2f(400 * healthPercent, 30));

    if (healthPercent > 0.6f) {
        healthBarForeground.setFillColor(Color::Green);
    }
    else if (healthPercent > 0.3f) {
        healthBarForeground.setFillColor(Color::Yellow);
    }
    else {
        healthBarForeground.setFillColor(Color::Red);
    }

    std::string healthString = "Health: " + std::to_string(currentHealth) + "/" + std::to_string(maxHealth);
    healthText->setString(healthString);

    updatePosition();
}

void HealthBar::draw(RenderWindow& window)
{
    window.draw(healthBarBackground);
    window.draw(healthBarForeground);
    window.draw(*healthText);
}

void HealthBar::updatePosition()
{
    Vector2f position = ScreenConfig::pos(1500, 20);
    healthBarBackground.setPosition(position);
    healthBarForeground.setPosition(position);
    healthText->setPosition(ScreenConfig::pos(1500, 60));
}