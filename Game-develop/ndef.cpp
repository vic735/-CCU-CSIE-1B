#ifndef CHICKENTIMER_H
#define CHICKENTIMER_H

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

class ChickenTimer
{
private:
    std::chrono::steady_clock::time_point startTime;

public:
    ChickenTimer()
    {
        startTime = std::chrono::steady_clock::now();
    }

    int getSurvivalTime()
    {
        auto now = std::chrono::steady_clock::now();

        return std::chrono::duration_cast
            <std::chrono::seconds>
            (now - startTime).count();
    }

    int getCoins()
    {
        // 每10秒獲得1金幣
        return getSurvivalTime() / 10;
    }

    void saveToJson(const std::string& fileName)
    {
        std::ofstream file(fileName);

        if (!file.is_open())
        {
            std::cout << "無法開啟檔案\n";
            return;
        }

        file << "{\n";
        file << "  \"survival_time\": "
            << getSurvivalTime() << ",\n";
        file << "  \"coins\": "
            << getCoins() << "\n";
        file << "}";

        file.close();
    }
};

#endif