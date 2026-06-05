//數值管理實作

#include "PetStats.h"
#include <string>
#include "json.hpp"    // 引入 JSON 函式庫
#include <fstream>
#include<iostream>

using json = nlohmann::json;
PetStats::PetStats(){
    hunger = 50.0f;
    mood = 80.0f;
    timet = 0.0f;
    survivalTime = 0; //存活計數器
    health = 100.0f;

    coins = 0;
    foodCount = 3;
    maxHealth = 100.0f; //預設上限值
    maxHunger = 100.0f;
    maxMood = 100.0f;
    highScore = 0;  //新增初始值

    jellyCount = 1; 
    jerkyCount = 1;
    pillCount = 1;





    LoadSettings(); // 遊戲啟動時自動讀檔
    
}

PetStats::~PetStats() {
    SaveSettings(); // 遊戲關閉時自動存檔！
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

void PetStats::Update(bool isChasing){
    timet+=GetFrameTime();  //計時器增加

    if(timet >= 1.0f){
        hunger -= 2.0f ;  //1s -2飢餓
        survivalTime += 1; 
        if (survivalTime > highScore) {
        highScore = survivalTime;
         SaveSettings(); //破紀錄立刻寫入 JSON 存檔
        }

        if (isChasing) {
            mood -= 2.0f; // 追逐飼料時：心情逐漸減少
        } else {
            mood += 1.0f; // 沒追逐時：心情自動回復
        }

        if (hunger < 30.0f || mood < 30.0f) {
            health -= 5.0f;
        }//扣血

        timet = 0.0f ;  //計時器歸零，重新計算

        //保全機制，確保數值不會超出範圍
        if (hunger < 0) hunger = 0;
        if (mood < 0) mood = 0; 
        if (mood > maxMood) mood = maxMood;
    }
}

void PetStats::TakeDamage(float amount){
    health -= amount;
    if (health < 0) health = 0;
}

int PetStats::GetFoodCount() { return foodCount; }
int PetStats::GetCoins() { return coins; }

void PetStats::DrawUI(int x, int y, Font font){
    //統一解釋，劃出飢餓度跟心情 反正就是讓數值UI化

    DrawTextEx(font, u8"飢餓度:", Vector2{(float)x, (float)y}, 24, 2, DARKGRAY);
    DrawRectangle(x + 100, y + 5, 100, 15, LIGHTGRAY); 
    DrawRectangle(x + 100, y + 5, (int)((hunger / maxHunger) * 100.0f), 15, ORANGE);

    DrawTextEx(font, u8"心情值:", Vector2{(float)x, (float)y + 30}, 24, 2, DARKGRAY);
    DrawRectangle(x + 100, y + 35, 100, 15, LIGHTGRAY);
    DrawRectangle(x + 100, y + 35, (int)((mood / maxMood) * 100.0f), 15, PINK);

    DrawTextEx(font, u8"血量:", Vector2{(float)x, (float)y + 60}, 24, 2, DARKGRAY);
    int healthBarWidth = (int)((health / maxHealth) * 100.0f);
    if (healthBarWidth < 0) healthBarWidth = 0;
    DrawRectangle(x + 100, y + 65, healthBarWidth, 15, RED);

    // 存活時間往下移一行 (+90)
    std::string timeText = u8"存活時間: " + std::to_string(survivalTime) + u8" 秒";
    DrawTextEx(font, timeText.c_str(), Vector2{(float)x, (float)y + 90}, 24, 2, DARKGRAY);
}

void PetStats::AddHunger(float amount){  //吃東西機制

    hunger += amount;
    if (hunger > maxHunger) hunger = maxHunger;
}

void PetStats::AddMood(float amount){//不用解釋吧......
    mood += amount;
    if (mood > maxMood) mood = maxMood;
}

float PetStats::GetHunger() { return hunger; }
float PetStats::GetMood() { return mood; }
int PetStats::GetSurvivalTime() { return survivalTime; }
float PetStats::GetHealth() { return health; }

// === 核心功能：JSON 存檔 ===
void PetStats::SaveSettings() {
    json saveJson;
    saveJson["hunger"] = hunger;
    saveJson["mood"] = mood;
    saveJson["coins"] = coins;
    saveJson["foodCount"] = foodCount;
    saveJson["highScore"] = highScore;
    saveJson["maxHealth"] = maxHealth;
    saveJson["maxHunger"] = maxHunger;
    saveJson["maxMood"] = maxMood;
    saveJson["jellyCount"] = jellyCount;
    saveJson["jerkyCount"] = jerkyCount;
    saveJson["pillCount"] = pillCount;

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
        highScore = loadJson.value("highScore", 0);
        maxHealth = loadJson.value("maxHealth", 100.0f);
        maxHunger = loadJson.value("maxHunger", 100.0f);
        maxMood = loadJson.value("maxMood", 100.0f);
        jellyCount = loadJson.value("jellyCount", 1);
        jerkyCount = loadJson.value("jerkyCount", 1);
        pillCount = loadJson.value("pillCount", 1);
        
        inFile.close();
    } catch (json::parse_error& e) {
        std::cerr << "讀檔失敗，格式毀損: " << e.what() << std::endl;
    }
}

void PetStats::Reset() {
    hunger = maxHunger;
    mood = maxMood;
    health = maxHealth;
    survivalTime = 0;
    SaveSettings(); // ⚠️ 重點：立刻用這組健康數值把 savegame.json 的死亡存檔蓋掉！
}

int PetStats::GetHighScore() { return highScore; }


float PetStats::GetMaxHealth() { return maxHealth; }
float PetStats::GetMaxHunger() { return maxHunger; }
float PetStats::GetMaxMood() { return maxMood; }

bool PetStats::BuyUpgradeMaxHealth(int price, float amount) {
    if (coins >= price) {
        coins -= price;
        maxHealth += amount;
        health = maxHealth; // 升級完順便補滿血
        SaveSettings();
        return true; // 升級成功
    }
    return false; // 錢不夠
}

bool PetStats::BuyUpgradeMaxHunger(int price, float amount) {
    if (coins >= price) {
        coins -= price;
        maxHunger += amount;
        hunger = maxHunger; // 升級完順便補滿飽食度
        SaveSettings();
        return true;
    }
    return false;
}

bool PetStats::BuyUpgradeMaxMood(int price, float amount) {
    if (coins >= price) {
        coins -= price;
        maxMood += amount;
        mood = maxMood; // 升級完順便補滿心情
        SaveSettings();
        return true;
    }
    return false;
}

void PetStats::EatSpecialFood(int type) {
    if (type == 1) {
        // 1. 極樂果凍：心情全滿、扣 25% 飽食、扣 20% 血量
        mood = maxMood;
        hunger -= maxHunger * 0.25f;
        health -= maxHealth * 0.20f;
    } 
    else if (type == 2) {
        // 2. 魔能肉乾：飽食全滿、扣 25% 心情、扣 20% 血量
        hunger = maxHunger;
        mood -= maxMood * 0.25f;
        health -= maxHealth * 0.20f;
    } 
    else if (type == 3) {
        // 3. 精煉大補丸：血量補 10 滴、飽食與心情各扣 50%
        health += 10.0f;
        hunger -= maxHunger * 0.50f;
        mood -= maxMood * 0.50f;
    }
    else if (type == 4) {
        // 4. 基礎飼料：單純安全補給，無副作用
        hunger += 30.0f;
        mood += 5.0f;
    }

    // ⚠️ 安全防爆牆：確保數值永遠落在安全範圍
    if (health > maxHealth) health = maxHealth;
    if (hunger > maxHunger) hunger = maxHunger;
    if (mood > maxMood) mood = maxMood;

    if (health < 0) health = 0;
    if (hunger < 0) hunger = 0;
    if (mood < 0) mood = 0;

    SaveSettings(); // 數值變動立刻存檔
}

//庫存控制
int PetStats::GetJellyCount() { return jellyCount; }
int PetStats::GetJerkyCount() { return jerkyCount; }
int PetStats::GetPillCount() { return pillCount; }

void PetStats::AddStock(int type, int amount) {
    if (type == 1) jellyCount += amount;
    else if (type == 2) jerkyCount += amount;
    else if (type == 3) pillCount += amount;
    SaveSettings();
}

bool PetStats::UseStock(int type) {
    if (type == 1 && jellyCount > 0) { jellyCount--; SaveSettings(); return true; }
    if (type == 2 && jerkyCount > 0) { jerkyCount--; SaveSettings(); return true; }
    if (type == 3 && pillCount > 0) { pillCount--; SaveSettings(); return true; }
    return false;
}