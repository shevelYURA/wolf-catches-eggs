#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class PlayersAttack
{
	Vector2f atk_Position;
	CircleShape atk_Egg;
	bool atk_InFlight = false;
	float atk_EggSpeed = 1000;
	Vector2f atk_EggDistance;
	Vector2f atk_Max;
	Vector2f atk_Min;
	
public:
	PlayersAttack();
	void stop();
	bool isInFlight();
	void throwEgg(float startX, float startY,
		float targetX, float targetY);
	FloatRect getPosition();
	CircleShape getShape();
	void update(float elapedTime);
};

