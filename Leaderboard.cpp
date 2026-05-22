#include "Leaderboard.h"
#include <fstream>
#include <string>
#include <algorithm>
#include "screenConfig.h"

Leaderboard::Leaderboard(sf::Font& f) : font(&f), visible(false) {
    background.setSize(ScreenConfig::size(600, 500));
    background.setFillColor(sf::Color(0, 0, 0, 220));
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(2);
    background.setPosition(ScreenConfig::pos(660, 200));
        
    titleText = new sf::Text(*font);
    titleText->setString("=== TOP SCORES ===");
    titleText->setCharacterSize(ScreenConfig::fontSize(36));
    titleText->setFillColor(sf::Color::Yellow);
    titleText->setOutlineColor(sf::Color::Black);
    titleText->setOutlineThickness(1);
    titleText->setPosition(ScreenConfig::pos(960, 230));
    titleText->setOrigin(sf::Vector2f(titleText->getLocalBounds().size.x / 2, titleText->getLocalBounds().size.y / 2));

    for (int i = 0; i < 10; i++) {
        rankTexts[i] = new sf::Text(*font);
        rankTexts[i]->setCharacterSize(ScreenConfig::fontSize(24));
        rankTexts[i]->setFillColor(sf::Color::White);
        rankTexts[i]->setPosition(ScreenConfig::pos(700.0f, 280.0f + i * 40.0f));

        nameTexts[i] = new sf::Text(*font);
        nameTexts[i]->setCharacterSize(ScreenConfig::fontSize(24));
        nameTexts[i]->setFillColor(sf::Color::White);
        nameTexts[i]->setPosition(ScreenConfig::pos(760.0f, 280.0f + i * 40.0f));

        scoreTexts[i] = new sf::Text(*font);
        scoreTexts[i]->setCharacterSize(ScreenConfig::fontSize(24));
        scoreTexts[i]->setFillColor(sf::Color::Yellow);
        scoreTexts[i]->setPosition(ScreenConfig::pos(1050.0f, 280.0f + i * 40.0f));
    }
}

Leaderboard::~Leaderboard() {
    delete titleText;
    for (int i = 0; i < 10; i++) {
        delete rankTexts[i];
        delete nameTexts[i];
        delete scoreTexts[i];
    }
}

void Leaderboard::show() { visible = true; }
void Leaderboard::hide() { visible = false; }
void Leaderboard::toggle() { visible = !visible; }
bool Leaderboard::isVisible() const { return visible; }

void Leaderboard::updateFromFile() {
    std::string names[100];
    int scores[100];
    int count = 0;

    std::ifstream file("leaderboard.txt");
    if (file.is_open()) {
        while (file >> names[count] >> scores[count] && count < 100) {
            count++;
        }
        file.close();
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (scores[i] < scores[j]) {
                int tempScore = scores[i];
                scores[i] = scores[j];
                scores[j] = tempScore;
                std::string tempName = names[i];
                names[i] = names[j];
                names[j] = tempName;
            }
        }
    }

    for (int i = 0; i < 10; i++) {
        if (i < count) {
            rankTexts[i]->setString(std::to_string(i + 1) + ".");
            nameTexts[i]->setString(names[i]);
            scoreTexts[i]->setString(std::to_string(scores[i]));
        }
        else {
            rankTexts[i]->setString(std::to_string(i + 1) + ".");
            nameTexts[i]->setString("---");
            scoreTexts[i]->setString("0");
        }
    }
}

void Leaderboard::draw(sf::RenderWindow& window) {
    if (!visible) return;

    window.draw(background);
    window.draw(*titleText);
    for (int i = 0; i < 10; i++) {
        window.draw(*rankTexts[i]);
        window.draw(*nameTexts[i]);
        window.draw(*scoreTexts[i]);
    }
}