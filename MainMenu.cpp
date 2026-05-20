#include "MainMenu.h"

MainMenu::MainMenu(sf::Font& fnt) : font(fnt), visible(false), showLeaderboard(false) {
    startText.setFont(font);
    startText.setString("START");
    startText.setCharacterSize(48);
    startText.setFillColor(sf::Color::White);
    startText.setOutlineColor(sf::Color::Black);
    startText.setOutlineThickness(2);
    startText.setOrigin(startText.getLocalBounds().width / 2.f, startText.getLocalBounds().height / 2.f);
    startText.setPosition(960, 400);

    leaderboardText.setFont(font);
    leaderboardText.setString("LEADERBOARD");
    leaderboardText.setCharacterSize(36);
    leaderboardText.setFillColor(sf::Color::White);
    leaderboardText.setOutlineColor(sf::Color::Black);
    leaderboardText.setOutlineThickness(2);
    leaderboardText.setOrigin(leaderboardText.getLocalBounds().width / 2.f, leaderboardText.getLocalBounds().height / 2.f);
    leaderboardText.setPosition(960, 520);

    exitText.setFont(font);
    exitText.setString("EXIT");
    exitText.setCharacterSize(32);
    exitText.setFillColor(sf::Color::White);
    exitText.setOutlineColor(sf::Color::Black);
    exitText.setOutlineThickness(2);
    exitText.setPosition(1700, 980);
}

void MainMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (!visible) return;
    if (event.is<sf::Event::MouseButtonPressed>()) {
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        if (startText.getGlobalBounds().contains(mouse.x, mouse.y)) {
            visible = false;
            showLeaderboard = false;
        } else if (leaderboardText.getGlobalBounds().contains(mouse.x, mouse.y)) {
            showLeaderboard = true;
            loadLeaderboard();
        } else if (exitText.getGlobalBounds().contains(mouse.x, mouse.y)) {
            window.close();
        }
    }
}

void MainMenu::draw(sf::RenderWindow& window) {
    if (!visible) return;
    window.draw(startText);
    window.draw(leaderboardText);
    window.draw(exitText);
    if (showLeaderboard) {
        for (auto& line : leaderboardLines) {
            window.draw(line);
        }
    }
}

void MainMenu::show() { visible = true; showLeaderboard = false; leaderboardLines.clear(); }
void MainMenu::hide() { visible = false; showLeaderboard = false; }
bool MainMenu::isVisible() const { return visible; }

void MainMenu::loadLeaderboard() {
    firebase.getTopScores(10, [this](const std::vector<LeaderboardEntry>& entries) {
        leaderboardLines.clear();
        float y = 250;
        int rank = 1;
        for (const auto& e : entries) {
            sf::Text line(font);
            line.setString(std::to_string(rank) + ". " + e.name + " - " + std::to_string(e.score));
            line.setCharacterSize(28);
            line.setFillColor(sf::Color::Yellow);
            line.setOutlineColor(sf::Color::Black);
            line.setOutlineThickness(1);
            line.setPosition(200, y);
            leaderboardLines.push_back(line);
            y += 35;
            rank++;
        }
    });
}
