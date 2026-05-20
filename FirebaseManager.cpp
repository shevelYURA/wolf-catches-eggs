#include "FirebaseManager.h"
#include <iostream>
#include <fstream>
#include <algorithm>

FirebaseManager::FirebaseManager() {
    std::cout << "FirebaseManager ready" << std::endl;
}

void FirebaseManager::saveScore(const std::string& playerName, int score) {
    std::ofstream file("score.json");
    file << "{\"name\":\"" << playerName << "\",\"score\":" << score << "}";
    file.close();

    system("curl -X POST \"https://wolfandeggs-407d9-default-rtdb.firebaseio.com/scores.json\" -H \"Content-Type: application/json\" -d @score.json");

    std::cout << "Score sent: " << playerName << " - " << score << std::endl;
}

void FirebaseManager::getTopScores(int limit, std::function<void(const std::vector<LeaderboardEntry>&)> callback) {
    std::string url = "https://wolfandeggs-407d9-default-rtdb.firebaseio.com/scores.json?orderBy=\"score\"&limitToLast=" + std::to_string(limit);
    std::string command = "curl -s \"" + url + "\" > scores_tmp.json";
    system(command.c_str());

    std::ifstream file("scores_tmp.json");
    std::vector<LeaderboardEntry> entries;
    
    if (file.is_open()) {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        
        size_t pos = 0;
        while ((pos = content.find("\"name\"", pos)) != std::string::npos) {
            size_t nameStart = content.find("\"", pos + 7) + 1;
            size_t nameEnd = content.find("\"", nameStart);
            std::string name = content.substr(nameStart, nameEnd - nameStart);
            
            size_t scorePos = content.find("\"score\"", nameEnd);
            size_t scoreStart = content.find(":", scorePos) + 1;
            size_t scoreEnd = content.find_first_of(",}", scoreStart);
            int score = std::stoi(content.substr(scoreStart, scoreEnd - scoreStart));
            
            entries.push_back({name, score});
            pos = scoreEnd;
        }
        
        std::sort(entries.begin(), entries.end(), [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
            return a.score > b.score;
        });
        if (entries.size() > limit) entries.resize(limit);
    }
    
    callback(entries);
}
