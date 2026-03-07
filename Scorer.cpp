#include "Scorer.h"

Scorer::Scorer() : score(0), scoreText(nullptr)
{
	if (!font.openFromFile("image/ARCADECLASSIC.ttf")) {}

    scoreText = new sf::Text(font);

    scoreText->setString("Score     0");
    scoreText->setCharacterSize(36);
    scoreText->setFillColor(sf::Color::White);
    scoreText->setOutlineColor(sf::Color::Black);
    scoreText->setOutlineThickness(2);
    scoreText->setPosition(sf::Vector2f(20, 20));
}

Scorer::~Scorer()
{
    delete scoreText;
}

void Scorer::addScore(int points)
{
    score += points;
    updateText();
}

void Scorer::reset()
{
    score = 0;
    updateText();
}

void Scorer::draw(sf::RenderWindow& window)
{
    if (scoreText) {
        window.draw(*scoreText);
    }
}

int Scorer::getScore() const
{
    return score;
}

void Scorer::updateText()
{
    if (scoreText) {
        scoreText->setString("Score     " + std::to_string(score));
    }
}