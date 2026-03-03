#include "Eggs.h"

Eggs::Eggs() : WorldObject(texEggs)
{
	if (!texEggs.loadFromFile("image/eggsTexture.png")) {
		throw;
	}
	WorldObject.setTexture(texEggs);
	WorldObject.setTextureRect(IntRect({ 262, 325 }, { 55, 50 }));
	restart();
}

Eggs::~Eggs()
{

}

void Eggs::move(float& time)
{
	switch (currentState) {
		case diagonal:
			//диагональное движение
			if (spawnSide == left) {
				WorldObject.move({ speed * time * 0.7f, speed * time * 0.5f });
			}
			else {
				WorldObject.move({ -speed * time * 0.7f, speed * time * 0.5f });
			}

			diagonalDistance += speed * time;

			//переключение состояния на падение
			if (diagonalDistance >= MAX_DIAGONAL_DISTANCE) {
				currentState = falling;
			}
			break;

		case falling:
			//падение
			WorldObject.move({ 0.0f, speed * time * 1.5f });
			break;
	}
	//выход за пределы(Потенциальное разбитие об пол)
	if (WorldObject.getPosition().y > 720 ||
		WorldObject.getPosition().x < -50 ||
		WorldObject.getPosition().x > 1330) {
		restart();
	}
}

void Eggs::draw(RenderWindow& window)
{
	window.draw(WorldObject);
}

bool Eggs::collision(FloatRect object)
{
	if (WorldObject.getGlobalBounds().findIntersection(object)) return true;
	return false;
}

void Eggs::restart()
{
	speed = static_cast<float>(0.5f); // 0.26 - 0.66
	spawnSide = (rand() % 2 == 0) ? left : right;
	float x = (spawnSide == left) ? leftSpawnX : rightSpawnX;
	float y = static_cast<float>(rand() % static_cast<int>(maxSpawnY - minSpawnY) + static_cast<int>(minSpawnY));

	WorldObject.setPosition(Vector2f{ x, y });

	currentState = diagonal;
	diagonalDistance = 0.0f;
}

FloatRect Eggs::Get_Eggs_Bound()
{
	return WorldObject.getGlobalBounds();
}