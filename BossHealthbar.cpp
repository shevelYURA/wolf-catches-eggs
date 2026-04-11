#include "BossHealthBar.h"
#include "ResourceManager.h"

BossHealthBar::BossHealthBar() : healthText(nullptr), active(false)
{
    font = ResourceManager::getFont(0);

    healthText = new Text(font);

    healthBarBackground.setSize(Vector2f(600, 40));
    healthBarBackground.setFillColor(Color(100, 100, 100));
    healthBarBackground.setOutlineColor(Color::Black);
    healthBarBackground.setOutlineThickness(2);

    healthBarForeground.setSize(Vector2f(600, 40));
    healthBarForeground.setFillColor(Color(200, 50, 50));

    healthText->setString("BOSS Health: 1000/1000");
    healthText->setCharacterSize(36);
    healthText->setFillColor(Color::White);
    healthText->setOutlineColor(Color::Black);
    healthText->setOutlineThickness(1);

    updatePosition();
}

BossHealthBar::~BossHealthBar()
{
    delete healthText;
}

void BossHealthBar::update(int currentHealth, int maxHealth)
{
    if (!active) return;

    float healthPercent = static_cast<float>(currentHealth) / maxHealth;
    healthBarForeground.setSize(Vector2f(600 * healthPercent, 40));

    std::string healthString = "BOSS Health: " + std::to_string(currentHealth) + "/" + std::to_string(maxHealth);
    healthText->setString(healthString);

    updatePosition();
}

void BossHealthBar::draw(RenderWindow& window)
{
    if (!active) return;

    window.draw(healthBarBackground);
    window.draw(healthBarForeground);
    window.draw(*healthText);
}

void BossHealthBar::setActive(bool active)
{
    this->active = active;
}

bool BossHealthBar::isActive() const
{
    return active;
}

void BossHealthBar::updatePosition()
{
    Vector2f position(660, 20);

    healthBarBackground.setPosition(position);
    healthBarForeground.setPosition(position);
    healthText->setPosition(Vector2f(position.x, position.y + 50));
}