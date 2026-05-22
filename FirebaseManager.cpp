#include "FirebaseManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

FirebaseManager::FirebaseManager() {
    databaseUrl = "https://wolfandeggs-407d9-default-rtdb.firebaseio.com/";
    std::cout << "FirebaseManager ready" << std::endl;
}

void FirebaseManager::clearAllScores() {
    std::string deleteCmd = "curl -X DELETE \"" + databaseUrl + "scores.json\" 2>nul";
    system(deleteCmd.c_str());
    std::cout << "All scores cleared" << std::endl;
}

int FirebaseManager::getCurrentBest(const std::string& playerName) {
    std::string cmd = "curl -s \"" + databaseUrl + "players/" + playerName + ".json\" > current_best.txt";
    system(cmd.c_str());

    std::ifstream file("current_best.txt");
    std::string scoreStr;
    int score = -1;

    if (file.is_open()) {
        std::getline(file, scoreStr);
        file.close();
        if (!scoreStr.empty() && scoreStr != "null") {
            try {
                score = std::stoi(scoreStr);
            }
            catch (...) {}
        }
    }
    return score;
}

void FirebaseManager::saveScore(const std::string& playerName, int score) {
    int currentBest = getCurrentBest(playerName);

    if (score <= currentBest && currentBest != -1) {
        std::cout << "Score not saved - not a new record" << std::endl;
        return;
    }

    if (currentBest != -1) {
        std::string findCmd = "curl -s \"" + databaseUrl + "scores.json?orderBy=\\\"name\\\"&equalTo=\\\"" + playerName + "\\\"\" > old_score.json";
        system(findCmd.c_str());

        std::ifstream file("old_score.json");
        std::string content;
        if (file.is_open()) {
            std::getline(file, content);
            file.close();
        }

        size_t quote1 = content.find("\"");
        if (quote1 != std::string::npos) {
            size_t quote2 = content.find("\"", quote1 + 1);
            if (quote2 != std::string::npos) {
                std::string key = content.substr(quote1 + 1, quote2 - quote1 - 1);
                if (!key.empty()) {
                    std::string deleteCmd = "curl -X DELETE \"" + databaseUrl + "scores/" + key + ".json\" 2>nul";
                    system(deleteCmd.c_str());
                    std::cout << "Deleted old score with key: " << key << std::endl;
                }
            }
        }
    }

    std::ofstream file("score.json");
    file << "{\"name\":\"" << playerName << "\",\"score\":" << score << "}";
    file.close();

    std::string postCmd = "curl -X POST \"" + databaseUrl + "scores.json\" -H \"Content-Type: application/json\" -d @score.json 2>nul";
    system(postCmd.c_str());

    std::string putCmd = "curl -X PUT \"" + databaseUrl + "players/" + playerName + ".json\" -H \"Content-Type: application/json\" -d \"" + std::to_string(score) + "\" 2>nul";
    system(putCmd.c_str());

    std::cout << "NEW RECORD saved: " << playerName << " - " << score << std::endl;
}