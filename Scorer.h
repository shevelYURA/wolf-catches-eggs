#pragma once
#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;

class Scorer
{
public:
    Scorer();
    ~Scorer();

    void addScore(int points);
    void reset();
    void draw(RenderWindow& window);
    int getScore() const;

private:
    int score;
    Font font;
    Text* scoreText;

    void updateText();
};

