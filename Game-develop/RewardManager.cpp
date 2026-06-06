#include "RewardManager.h"
#include <fstream>

RewardManager::RewardManager(int ratio) {
    coinRatio = ratio;
}

RewardManager::~RewardManager() {
}

int RewardManager::CalculateEarnedCoins(int survivalTime) {
    return survivalTime * coinRatio;
}

void RewardManager::SaveSettlementJson(const std::string& filename, const std::string& playerName, int survivalTime, int totalCoins) {
    std::ofstream saveFile(filename);
    if (saveFile.is_open()) {
        saveFile << "{\n"
                 << "  \"player\": \"" << playerName << "\",\n"
                 << "  \"survivalTime\": " << survivalTime << ",\n"
                 << "  \"earnedCoins\": " << CalculateEarnedCoins(survivalTime) << ",\n"
                 << "  \"totalCoins\": " << totalCoins << "\n"
                 << "}";
        saveFile.close();
    }
}