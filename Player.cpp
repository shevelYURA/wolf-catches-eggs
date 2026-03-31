#include "Player.h"

Player::Player() : health(100), attackKeyPressed(false)
{
    if (!texWolf.loadFromFile("image/wolfTexture1.png")) {
        throw;
    }
    if (!texBasket.loadFromFile("image/basketTexture.png")) {
        throw;
    }

    wolf.setSize(Vector2f(150, 150));
    wolf.setTexture(&texWolf);
    wolf.setPosition(Vector2f(910, 920));

    basket.setSize(Vector2f(50, 15));
    basket.setTexture(&texBasket);
}

void Player::update(float time, const RenderWindow& window)
{
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
    if (pos.x > 1920 - 50) {
        wolf.setPosition(Vector2f(1920 - 50, pos.y));
    }
    if (pos.x < 0) {
        wolf.setPosition(Vector2f(0, pos.y));
    }

    basket.setPosition(Vector2f(wolf.getPosition().x, wolf.getPosition().y + 15 * 8));

    handleAttack(window);

    attack.update(time);
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