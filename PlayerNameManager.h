#pragma once
#include <string>

class PlayerNameManager {
private:
    std::string playerName;
    int personalBest;

    void loadFromFile();
    void saveToFile();

public:
    PlayerNameManager();
    std::string getName() const;
    void setName(const std::string& newName);
    int getPersonalBest() const;
    void updatePersonalBest(int score);
};