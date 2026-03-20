#include "Player.h"

Player::Player() : health(100)
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

void Player::update(float time)
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
}

void Player::draw(RenderWindow& window)
{
    window.draw(wolf);
    window.draw(basket);
}

FloatRect Player::getBasketBounds() const
{
    return basket.getGlobalBounds();
}

int Player::getHealth() const
{
    return health;
}

void Player::takeDamage(int damage)
{
    health -= damage;
    if (health < 0) health = 0;
    checkHealth();
}

bool Player::isAlive() const
{
    return health > 0;
}

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