#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Bombs
{
public:
    Bombs();
    ~Bombs();

    void move(float time);
    void draw(RenderWindow& window);
    bool collision(FloatRect object);
    void restart();
    FloatRect Get_Bombs_Bound();

private:
    RectangleShape bomb;
    Texture texBombs;

    enum BombState {
        falling,
        waiting
    };

    BombState currentState;
    float speed;
    float waitTimer;
    float waitTime;
};