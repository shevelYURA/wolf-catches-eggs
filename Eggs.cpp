#include "Eggs.h"

Eggs::Eggs() : WorldObject(texEggs)
{
	if (!texEggs.loadFromFile("image/eggsTexture.jpg")) {
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
	WorldObject.move({ -0.5f * time, 0.0f });
	if (WorldObject.getPosition().x < -60) restart();
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
	float s = static_cast<float>((rand() % 13 + 5) / 10.0f);
	float x = static_cast<float>(rand() % 1280 + 1280);
	float y = static_cast<float>(rand() % 541 + 130);

	WorldObject.setPosition(Vector2f{x, y});
	WorldObject.setScale(Vector2f{ s, s });
	ix = rand() % 4;
	iy = rand() % 5;
	st = rand() % 2;
}

FloatRect Eggs::Get_Eggs_Bound()
{
	return WorldObject.getGlobalBounds();
}