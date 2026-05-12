#pragma once
#include <string>

class FirebaseManager {
private:
    std::string databaseUrl;
    int getCurrentBest(const std::string& playerName);
    std::string findOldScoreKey(const std::string& playerName);

public:
    FirebaseManager();
    void saveScore(const std::string& playerName, int score);
    void clearAllScores();
};