#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class PlayersAttack
{
private:
	Vector2f atk_Position;
	CircleShape atk_Egg;
	bool atk_InFlight = false;
	float atk_EggSpeed = 1000;
	sf::Vector2f atk_EggDistance;
	sf::Vector2f atk_Max;
	sf::Vector2f atk_Min;
	
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

