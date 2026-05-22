#pragma once
#include <string>

class FirebaseManager {
private:
    std::string databaseUrl;

public:
    FirebaseManager();
    void clearAllScores();
    void saveScore(const std::string& playerName, int score);
    int getCurrentBest(const std::string& playerName);
};