#include "PlayerNameManager.h"
#include <fstream>
#include <iostream>

PlayerNameManager::PlayerNameManager() : personalBest(0) {
    loadFromFile();
}

void PlayerNameManager::loadFromFile() {
    std::ifstream file("player_data.txt");
    if (file.is_open()) {
        std::getline(file, playerName);
        file >> personalBest;
        file.close();
    }

    if (playerName.empty()) {
        playerName = "WolfPlayer";
    }
}

void PlayerNameManager::saveToFile() {
    std::ofstream file("player_data.txt");
    file << playerName << std::endl;
    file << personalBest;
    file.close();
}

std::string PlayerNameManager::getName() const {
    return playerName;
}

void PlayerNameManager::setName(const std::string& newName) {
    playerName = newName;
    saveToFile();
}

int PlayerNameManager::getPersonalBest() const {
    return personalBest;
}

void PlayerNameManager::updatePersonalBest(int score) {
    if (score > personalBest) {
        personalBest = score;
        saveToFile();
    }
}