#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "FirebaseManager.h"

class MainMenu {
public:
    MainMenu(sf::Font& font);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void show();
    void hide();
    bool isVisible() const;

private:
    sf::Font& font;
    sf::Text startText, leaderboardText, exitText;
    std::vector<sf::Text> leaderboardLines;
    FirebaseManager firebase;
    bool visible;
    bool showLeaderboard;
    void loadLeaderboard();
};
