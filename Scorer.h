#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;

class Scorer
{
    int score;
    Font font;
    Text* scoreText;
    void updateText();

public:
    Scorer();
    ~Scorer();

    void addScore(int points);
    void reset();
    void draw(RenderWindow& window);
    int getScore() const;
};

