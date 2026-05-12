#include "FirebaseManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <regex>

FirebaseManager::FirebaseManager() {
    databaseUrl = "https://wolfandeggs-407d9-default-rtdb.firebaseio.com/";
    std::cout << "FirebaseManager ready" << std::endl;
}

void FirebaseManager::clearAllScores() {
    std::string deleteCmd = "curl -X DELETE \"" + databaseUrl + "scores.json\" 2>nul";
    system(deleteCmd.c_str());
    std::cout << "All scores cleared!" << std::endl;
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

std::string FirebaseManager::findOldScoreKey(const std::string& playerName) {
    std::string findCmd = "curl -s \"" + databaseUrl + "scores.json?orderBy=\\\"name\\\"&equalTo=\\\"" + playerName + "\\\"\" > old_score.json";
    system(findCmd.c_str());

    std::ifstream file("old_score.json");
    std::string content;
    if (file.is_open()) {
        std::getline(file, content);
        file.close();
    }

    if (content.empty() || content == "{}" || content == "null") {
        return "";
    }

    std::regex keyRegex("\"([a-zA-Z0-9_-]+)\"\\s*:\\s*\\{");
    std::smatch match;
    if (std::regex_search(content, match, keyRegex)) {
        return match[1].str();
    }

    return "";
}

void FirebaseManager::saveScore(const std::string& playerName, int score) {
    int currentBest = getCurrentBest(playerName);

    std::cout << "Current best in players: " << currentBest << ", New score: " << score << std::endl;

    if (score <= currentBest && currentBest != -1) {
        std::cout << "Score NOT saved - not a new record" << std::endl;
        return;
    }

    if (currentBest != -1) {
        std::string oldKey = findOldScoreKey(playerName);
        if (!oldKey.empty()) {
            std::string deleteCmd = "curl -X DELETE \"" + databaseUrl + "scores/" + oldKey + ".json\" 2>nul";
            system(deleteCmd.c_str());
            std::cout << "Deleted old score with key: " << oldKey << std::endl;
        }
    }

    std::ofstream scoreFile("score.json");
    scoreFile << "{\"name\":\"" << playerName << "\",\"score\":" << score << "}";
    scoreFile.close();

    std::string postCmd = "curl -X POST \"" + databaseUrl + "scores.json\" -H \"Content-Type: application/json\" -d @score.json 2>nul";
    system(postCmd.c_str());

    std::string putCmd = "curl -X PUT \"" + databaseUrl + "players/" + playerName + ".json\" -H \"Content-Type: application/json\" -d \"" + std::to_string(score) + "\" 2>nul";
    system(putCmd.c_str());

    std::cout << "NEW RECORD saved: " << playerName << " - " << score << std::endl;
}