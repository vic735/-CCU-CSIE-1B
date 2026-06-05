#include <iostream>
#include <thread>

#include "ChickenTimer.h"

int main()
{
    ChickenTimer chicken;

    while (true)
    {
        system("cls");   // Visual Studio Windows

        std::cout << "====== 電子雞 ======\n";
        std::cout << "存活時間: "
            << chicken.getSurvivalTime()
            << " 秒\n";

        std::cout << "金幣: "
            << chicken.getCoins()
            << "\n";

        chicken.saveToJson("data.json");

        std::cout << "\n資料已自動存檔\n";

        std::this_thread::sleep_for(
            std::chrono::seconds(1));
    }

    return 0;
}