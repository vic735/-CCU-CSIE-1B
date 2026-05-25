//數值管理實作

#include "PetStats.h"
#include "json.hpp"    // 引入 JSON 函式庫
#include <fstream>
#include<iostream>

using json = nlohmann::json;
PetStats::PetStats(){
    hunger = 50.0f;
    mood = 80.0f;
    timet = 0.0f;

    // 預設數值，如果有存檔會被覆蓋
    coins = 0;
    foodCount = 3; // 初始給 3 個飼料

    LoadSettings(); // 遊戲啟動時自動讀檔！
}

PetStats::~PetStats() {
    SaveSettings(); // 遊戲關閉時自動存檔！
}

// 實作：點擊獲取金幣
void PetStats::ClickForCoin() {
    coins += 10; // 點一次得 10 元
}

// 實作：商店購買
bool PetStats::BuyFood(int price) {
    if (coins >= price) {
        coins -= price;
        foodCount++;
        SaveSettings(); // 購買後即時存檔
        return true;
    }
    return false; // 錢不夠
}

// 實作：使用飼料
bool PetStats::UseFood() {
    if (foodCount > 0) {
        foodCount--;
        return true;
    }
    return false;
}

int PetStats::GetFoodCount() { return foodCount; }
int PetStats::GetCoins() { return coins; }

// === 核心功能：JSON 存檔 ===
void PetStats::SaveSettings() {
    json saveJson;
    saveJson["hunger"] = hunger;
    saveJson["mood"] = mood;
    saveJson["coins"] = coins;
    saveJson["foodCount"] = foodCount;

    std::ofstream outFile("savegame.json");
    if (outFile.is_open()) {
        outFile << saveJson.dump(4); // 縮排 4 空格美化
        outFile.close();
    }
}

// === 核心功能：JSON 讀檔 ===
void PetStats::LoadSettings() {
    std::ifstream inFile("savegame.json");
    if (!inFile.is_open()) {
        return; // 若沒有存檔（第一次玩），直接用建構子的預設值
    }

    try {
        json loadJson;
        inFile >> loadJson;

        // 使用 .value() 防止手殘改壞 JSON 導致當機，安全回退預設值
        hunger = loadJson.value("hunger", 50.0f);
        mood = loadJson.value("mood", 80.0f);
        coins = loadJson.value("coins", 0);
        foodCount = loadJson.value("foodCount", 3);
        
        inFile.close();
    } catch (json::parse_error& e) {
        std::cerr << "讀檔失敗，格式毀損: " << e.what() << std::endl;
    }
}

// 讓介面能順便看到金幣與飼料庫存
void PetStats::DrawUI(int x, int y, Font font) {
    DrawTextEx(font, u8"飢餓度:", Vector2{(float)x, (float)y}, 24, 2, DARKGRAY);
    DrawRectangle(x + 100, y + 5, 100, 15, LIGHTGRAY);
    DrawRectangle(x + 100, y + 5, (int)hunger, 15, ORANGE); 

    DrawTextEx(font, u8"心情值:", Vector2{(float)x, (float)y + 30}, 24, 2, DARKGRAY);
    DrawRectangle(x + 100, y + 35, 100, 15, LIGHTGRAY);
    DrawRectangle(x + 100, y + 35, (int)mood, 15, PINK);

    // 顯示錢與庫存
    std::string coinStr = u8"金幣: " + std::to_string(coins);
    std::string foodStr = u8"飼料庫存: " + std::to_string(foodCount);
    DrawTextEx(font, coinStr.c_str(), Vector2{(float)x + 250, (float)y}, 24, 2, GOLD);
    DrawTextEx(font, foodStr.c_str(), Vector2{(float)x + 250, (float)y + 30}, 24, 2, BLUE);
}

void PetStats::Update(){
    timet+=GetFrameTime();  //計時器增加

    if(timet >= 1.0f){
        hunger -= 2.0f ;  //1s -2飢餓
        mood -= 1.0f ;  //1s -1心情
        timet = 0.0f ;  //計時器歸零，重新計算

        if (hunger < 0) hunger = 0;
        if (mood < 0) mood = 0; //保全機制，最低是0
    }
}

void PetStats::AddHunger(float amount){  //吃東西機制

    hunger += amount;
    if (hunger > 100.0f) hunger = 100.0f;
}

void PetStats::AddMood(float amount){//不用解釋吧......
    mood += amount;
    if (mood > 100.0f) mood = 100.f;
}

float PetStats::GetHunger() { return hunger; }
float PetStats::GetMood() { return mood; }