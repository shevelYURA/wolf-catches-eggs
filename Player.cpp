#include "Player.h"
#include "ResourceManager.h"
#include "ScreenConfig.h"

Player::Player() : health(100), attackKeyPressed(false),
    boxingGloveActive(false), boxingGloveTimer(0), boxingGloveDuration(15.0f)
{
    texWolf = ResourceManager::getTexture(IDB_PNG1);
    texBasket = ResourceManager::getTexture(IDB_BASKET);

    wolf.setSize(ScreenConfig::size(150, 150));
    wolf.setTexture(&texWolf);
    wolf.setPosition(ScreenConfig::pos(910, 920));

    basket.setSize(ScreenConfig::size(100, 100));
    basket.setTexture(&texBasket);
}

void Player::update(float time, const RenderWindow& window)
{
    // Обновляем состояние буста
    updateBoxingGlove(time);
    
    const float SPEED = 600.0f;
    Vector2f moveRec(0.f, 0.f);

    if (Keyboard::isKeyPressed(Keyboard::Key::D)) {
        moveRec.x += SPEED * time;
    }
    if (Keyboard::isKeyPressed(Keyboard::Key::A)) {
        moveRec.x -= SPEED * time;
    }

    wolf.move(moveRec);

    Vector2f pos = wolf.getPosition();
    if (pos.x > ScreenConfig::scaleX * 1920 - ScreenConfig::scaleX * 150) {
        wolf.setPosition(Vector2f(ScreenConfig::scaleX * 1920 - ScreenConfig::scaleX * 150, pos.y));
    }
    if (pos.x < 0) {
        wolf.setPosition(Vector2f(0, pos.y));
    }

    basket.setPosition(Vector2f(wolf.getPosition().x+70, wolf.getPosition().y + 30));

    handleAttack(window);

    attack.update(time, window);
}

void Player::handleAttack(const RenderWindow& window)
{
    if (isAlive()) {
        bool isMousePressed = Mouse::isButtonPressed(Mouse::Button::Left);

        if (isMousePressed && !attackKeyPressed && !attack.isInFlight()) {
            Vector2i mousePos = Mouse::getPosition(window);
            Vector2f worldPos = window.mapPixelToCoords(mousePos);

            FloatRect basketBounds = basket.getGlobalBounds();
            float startX = basketBounds.position.x + basketBounds.size.x / 2.0f;
            float startY = basketBounds.position.y;

            attack.throwEgg(startX, startY, worldPos.x, worldPos.y);
            attackKeyPressed = true;
        }
        else if (!isMousePressed) {
            attackKeyPressed = false;
        }
    }
}

void Player::draw(RenderWindow& window)
{
    window.draw(wolf);
    window.draw(basket);
    if (attack.isInFlight()) { window.draw(attack.getShape()); }
}

FloatRect Player::getBasketBounds() const { return basket.getGlobalBounds(); }

FloatRect Player::getBounds() const { return wolf.getGlobalBounds(); }

Vector2f Player::getBottomCenter() const
{
    FloatRect bounds = wolf.getGlobalBounds();
    return Vector2f(bounds.position.x + bounds.size.x / 2.0f,
        bounds.position.y + bounds.size.y);
}

int Player::getHealth() const { return health; }

void Player::takeDamage(int damage)
{
    health -= damage;
    if (health < 0) health = 0;
    checkHealth();
}

bool Player::isAlive() const { return health > 0; }

void Player::reset()
{
    health = 100;
    wolf.setFillColor(Color::White);
    resetBoxingGlove();
}

void Player::checkHealth()
{
    if (health <= 0) {
        wolf.setFillColor(Color(255, 0, 0, 128));
    }
    else if (health < 30) {
        wolf.setFillColor(Color(255, 100, 100));
    }
    else {
        wolf.setFillColor(Color::White);
    }
}

// МЕТОДЫ ДЛЯ БУСТА "БОКСЁРСКАЯ ПЕРЧАТКА"
void Player::activateBoxingGlove() {
    boxingGloveActive = true;
    boxingGloveTimer = boxingGloveDuration;
}

void Player::updateBoxingGlove(float time) {
    if (!boxingGloveActive) return;
    
    boxingGloveTimer -= time;
    if (boxingGloveTimer <= 0) {
        boxingGloveActive = false;
    }
    // МЕТОД ДЛЯ ВОССТАНОВЛЕНИЯ ЗДОРОВЬЯ
void Player::heal(int amount) {
    health += amount;
    if (health > 100) health = 100;
    checkHealth();
}
}
