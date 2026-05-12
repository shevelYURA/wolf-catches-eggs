#pragma once
#include <string>

class FirebaseManager {
public:
    FirebaseManager();
    void saveScore(const std::string& playerName, int score);
};