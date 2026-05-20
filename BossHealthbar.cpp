#include "BossHealthBar.h"
#include "ResourceManager.h"
#include "screenConfig.h"

BossHealthBar::BossHealthBar() : healthText(nullptr), active(false)
{
    font = ResourceManager::getFont(0);

    healthText = new Text(font);

    healthBarBackground.setFillColor(Color(100, 100, 100));
    healthBarBackground.setOutlineColor(Color::Black);
    healthBarBackground.setOutlineThickness(2);

    healthBarForeground.setFillColor(Color(200, 50, 50));

    healthText->setString("BOSS Health: 1000/1000");
    healthText->setCharacterSize(ScreenConfig::fontSize(26));
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
    Vector2f barSize = ScreenConfig::size(600, 40);

    healthBarBackground.setSize(barSize);
    healthBarForeground.setSize(Vector2f(barSize.x * healthPercent, barSize.y));

    std::string healthString = "BOSS Health: " + std::to_string(currentHealth) + "/" + std::to_string(maxHealth);
    healthText->setString(healthString);
    healthText->setCharacterSize(ScreenConfig::fontSize(26));

    updatePosition();
}

void BossHealthBar::draw(RenderWindow& window)
{
    if (!active) return;

    updatePosition();

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
    Vector2f position = ScreenConfig::pos(660, 20);
    healthBarBackground.setPosition(position);
    healthBarForeground.setPosition(position);
    healthText->setPosition(ScreenConfig::pos(690, 25));
}