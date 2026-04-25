#include "FirebaseManager.h"
#include <iostream>
#include <fstream>

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