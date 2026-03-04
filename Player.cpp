#include "Player.h"

Player::Player()
{
    if (!texWolf.loadFromFile("image/wolfTexture.png")) {
        throw;
    }
    if (!texBasket.loadFromFile("image/basketTexture.png")) {
        throw;
    }

    wolf.setSize(Vector2f(50, 150));
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