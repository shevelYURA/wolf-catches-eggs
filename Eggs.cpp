#include "Eggs.h"

Eggs::Eggs()
{
	if (!texEggs.loadFromFile("image/eggsTexture.png")) {
		throw;
	}

	egg.setSize(Vector2f(40, 35));
	egg.setTexture(&texEggs);

	restart();

	speed = 70.0f;
}

Eggs::~Eggs()
{

}

void Eggs::move(float time)
{
	switch (currentState) {
	case waiting:
		waitTimer += time;
		if (waitTimer >= waitTime) {
			currentState = falling;
			float x = static_cast<float>(rand() % 1820 + 50);
			egg.setPosition(Vector2f{ x, 0.0f });
		}
		break;

	case falling:
		egg.move({ 0.0f, speed * time * 1.5f });
		break;
	}
	//выход за пределы(Потенциальное разбитие об пол)
	if (egg.getPosition().y > 1080) {
		restart();
	}
}

void Eggs::draw(RenderWindow& window)
{
	if (currentState == falling) {
		window.draw(egg);
	}
}

bool Eggs::collision(FloatRect object)
{
	if (currentState == falling) {
		return egg.getGlobalBounds().findIntersection(object).has_value();
	}
	return false;
}

void Eggs::restart()
{
	waitTime = (rand() % 5) + 1;
	waitTimer = 0;
	currentState = waiting;
	egg.setPosition(Vector2f{ -100, -100 });
}

FloatRect Eggs::Get_Eggs_Bound()
{
	return egg.getGlobalBounds();
}