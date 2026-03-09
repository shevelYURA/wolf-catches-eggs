#include "Bombs.h"

Bombs::Bombs()
{
	if (!texBombs.loadFromFile("image/bombTexture.png")) {
		throw;
	}
	bomb.setSize(Vector2f(40, 35));
	bomb.setTexture(&texBombs);

	restart();

	speed = 70.0f;
}

Bombs::~Bombs()
{

}

void Bombs::move(float time)
{
	switch (currentState) {
	case waiting:
		waitTimer += time;
		if (waitTimer >= waitTime) {
			currentState = falling;
			float x = static_cast<float>(rand() % 1820 + 50);
			bomb.setPosition(Vector2f{ x, 0.0f });
		}
		break;

	case falling:
		bomb.move({ 0.0f, speed * time * 1.5f });
		break;
	}
	if (bomb.getPosition().y > 1080) {
		restart();
	}
}

void Bombs::draw(RenderWindow& window)
{
	if (currentState == falling) {
		window.draw(bomb);
	}
}

bool Bombs::collision(FloatRect object)
{
	if (currentState == falling) {
		return bomb.getGlobalBounds().findIntersection(object).has_value();
	}
	return false;
}

void Bombs::restart()
{
	waitTime = (rand() % 5) + 1;
	waitTimer = 0;
	currentState = waiting;
	bomb.setPosition(Vector2f{ -100, -100 });
}

FloatRect Bombs::Get_Bombs_Bound()
{
	return bomb.getGlobalBounds();
}