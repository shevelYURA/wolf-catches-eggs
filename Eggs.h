#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Eggs
{
public:
    Eggs();
    ~Eggs();

    void move(float time);
    void draw(RenderWindow& window);
    bool collision(FloatRect object);
    void restart();
    FloatRect Get_Eggs_Bound();

private:
    Sprite WorldObject;
    Texture texEggs;

    int ix, iy = 0;
    int st = 0;

    enum EggState {
        falling,
        waiting
    };

    EggState currentState;
    float speed;
    float waitTimer;
    float waitTime;
};