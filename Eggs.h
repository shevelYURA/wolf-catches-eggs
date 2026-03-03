#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Eggs
{
public:
	Eggs();
	~Eggs();

	void move(float& time);
	void draw(RenderWindow& window);
	bool collision(FloatRect object);
	void restart();
	FloatRect Get_Eggs_Bound();

private:
	Sprite WorldObject;
	Texture texEggs;
	
	int xsp[5]{3, 73, 135, 198, 262};
	int ysp[6]{3, 68, 132, 200, 265, 325};
	int ix, iy = 0;
	int st = 0;

	enum Spawn_Side {
		left,
		right
	};

	enum EggState {
		diagonal,
		falling
	};

	Spawn_Side spawnSide;
	EggState currentState;

	float speed;
	float diagonalDistance;
	const float leftSpawnX = 0.0f;
	const float rightSpawnX = 1230.0f;
	const float minSpawnY = 100.0f;
	const float maxSpawnY = 300.0f;
	const float MAX_DIAGONAL_DISTANCE = 300.0f;
};

