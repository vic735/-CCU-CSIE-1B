#pragma once
#include <string>

class RewardManager {
private:
    int coinRatio; // 存活秒數換算金幣的比例

public:
    // 建構子，預設 1 秒換 2 元
    RewardManager(int ratio = 2);
    ~RewardManager();

    // 核心功能 1：依據存活秒數計算獲得的金幣
    int CalculateEarnedCoins(int survivalTime);

    // 核心功能 2：結算數據並將資料寫入 JSON 存檔
    void SaveSettlementJson(const std::string& filename, const std::string& playerName, int survivalTime, int totalCoins);
};