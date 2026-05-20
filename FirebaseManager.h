#pragma once
#include <string>
#include <vector>
#include <functional>

struct LeaderboardEntry {
    std::string name;
    int score;
};

class FirebaseManager {
public:
    FirebaseManager();
    void saveScore(const std::string& playerName, int score);
    void getTopScores(int limit, std::function<void(const std::vector<LeaderboardEntry>&)> callback);
};
