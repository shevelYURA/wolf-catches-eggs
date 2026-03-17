#include "PlayersAttack.h"

PlayersAttack::PlayersAttack()
{
	atk_Egg.setFillColor(sf::Color::White);
	atk_Egg.setRadius(10);
	atk_Egg.setOrigin({ 5, 5 });
}

void PlayersAttack::throwEgg(float startX, float startY,
	float targetX, float targetY) {

	atk_InFlight = true;
	atk_Position = { startX, startY };

	float dx = targetX - startX;
	float dy = targetY - startY;
	float distance = std::sqrt(dx * dx + dy * dy);
	if (distance > 0) {
		atk_EggDistance.x = (dx / distance) * atk_EggSpeed;
		atk_EggDistance.y = (dy / distance) * atk_EggSpeed;
	}

	float range = 1000;
	atk_Min = { startX - range, startY - range };
	atk_Max = { startX + range, startY + range };

	atk_Egg.setPosition(atk_Position);
}

void PlayersAttack::stop() { atk_InFlight = false; }

bool PlayersAttack::isInFlight() { return atk_InFlight; }

FloatRect PlayersAttack::getPosition() { return atk_Egg.getGlobalBounds(); }

CircleShape PlayersAttack::getShape() { return atk_Egg; }

void PlayersAttack::update(float elapsedTime) {
	if (atk_InFlight) {
		atk_Position.x += atk_EggDistance.x * elapsedTime;
		atk_Position.y += atk_EggDistance.y * elapsedTime;

		atk_Egg.setPosition(atk_Position);

		if (atk_Position.x < atk_Min.x || atk_Position.x > atk_Max.x ||
			atk_Position.y < atk_Min.y || atk_Position.y > atk_Max.y) {
			atk_InFlight = false;
		}
	}
}