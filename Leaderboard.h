#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Leaderboard {
private:
    sf::RectangleShape background;
    sf::Text* titleText;
    sf::Text* rankTexts[10];
    sf::Text* nameTexts[10];
    sf::Text* scoreTexts[10];
    sf::Font* font;
    bool visible;

public:
    Leaderboard(sf::Font& f);
    ~Leaderboard();
    void show();
    void hide();
    void toggle();
    bool isVisible() const;
    void updateFromFile();
    void draw(sf::RenderWindow& window);
};