#include "HealthBar.h"

HealthBar::HealthBar() : healthText(nullptr)  // Инициализируем указатель
{
    if (!font.openFromFile("image/ARCADECLASSIC.ttf")) {
        throw;
    }

    healthText = new sf::Text(font);

    // Настройка фона полоски здоровья
    healthBarBackground.setSize(sf::Vector2f(400, 30));
    healthBarBackground.setFillColor(sf::Color(100, 100, 100));
    healthBarBackground.setOutlineColor(sf::Color::Black);
    healthBarBackground.setOutlineThickness(2);

    // Настройка полоски здоровья
    healthBarForeground.setSize(sf::Vector2f(400, 30));
    healthBarForeground.setFillColor(sf::Color::Green);

    // Настройка текста здоровья
    healthText->setString("Health: 100/100");
    healthText->setCharacterSize(36);
    healthText->setFillColor(sf::Color::White);
    healthText->setOutlineColor(sf::Color::Black);
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
    healthBarForeground.setSize(sf::Vector2f(400 * healthPercent, 30));

    if (healthPercent > 0.6f) {
        healthBarForeground.setFillColor(sf::Color::Green);
    }
    else if (healthPercent > 0.3f) {
        healthBarForeground.setFillColor(sf::Color::Yellow);
    }
    else {
        healthBarForeground.setFillColor(sf::Color::Red);
    }

    std::string healthString = "Health: " + std::to_string(currentHealth) + "/" + std::to_string(maxHealth);
    healthText->setString(healthString);

    updatePosition();
}

void HealthBar::draw(sf::RenderWindow& window)
{
    window.draw(healthBarBackground);
    window.draw(healthBarForeground);
    window.draw(*healthText);
}

void HealthBar::updatePosition()
{
    sf::Vector2f position(1500, 20);

    healthBarBackground.setPosition(position);
    healthBarForeground.setPosition(position);
    healthText->setPosition(sf::Vector2f(position.x, position.y + 40));
}