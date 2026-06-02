//數值管理實作

#include "PetStats.h"
#include <string>

PetStats::PetStats(){
    hunger = 50.0f;
    mood = 80.0f;
    timet = 0.0f;
    survivalTime = 0; //存活計數器
    health = 100.0f;
}

PetStats::~PetStats(){

}

void PetStats::Update(bool isChasing){
    timet+=GetFrameTime();  //計時器增加

    if(timet >= 1.0f){
        hunger -= 2.0f ;  //1s -2飢餓
        survivalTime += 1; 

        if (isChasing) {
            mood -= 2.0f; // 追逐飼料時：心情逐漸減少
        } else {
            mood += 1.0f; // 沒追逐時：心情自動回復
        }

        if (hunger < 30.0f || mood < 30.0f) {
            health -= 5.0f;
        }//扣血

        timet = 0.0f ;  //計時器歸零，重新計算

        //保全機制，確保數值不會超出 0~100 的範圍
        if (hunger < 0) hunger = 0;
        if (mood < 0) mood = 0; 
        if (mood > 100.0f) mood = 100.0f;
    }
}

void PetStats::TakeDamage(float amount){
    health -= amount;
    if (health < 0) health = 0;
}

void PetStats::DrawUI(int x, int y, Font font){
    //統一解釋，劃出飢餓度跟心情 反正就是讓數值UI化

    DrawTextEx(font, u8"飢餓度:", Vector2{(float)x, (float)y}, 24, 2, DARKGRAY);
    DrawRectangle(x + 100, y + 5, 100, 15, LIGHTGRAY); // 100 是滿血長度
    DrawRectangle(x + 100, y + 5, (int)hunger, 15, ORANGE); 

    DrawTextEx(font, u8"心情值:", Vector2{(float)x, (float)y + 30}, 24, 2, DARKGRAY);
    DrawRectangle(x + 100, y + 35, 100, 15, LIGHTGRAY);
    DrawRectangle(x + 100, y + 35, (int)mood, 15, PINK);

    DrawTextEx(font, u8"血量:", Vector2{(float)x, (float)y + 60}, 24, 2, DARKGRAY);
    DrawRectangle(x + 100, y + 65, 100, 15, LIGHTGRAY);
    DrawRectangle(x + 100, y + 65, (int)health, 15, RED);

    // 存活時間往下移一行 (+90)
    std::string timeText = u8"存活時間: " + std::to_string(survivalTime) + u8" 秒";
    DrawTextEx(font, timeText.c_str(), Vector2{(float)x, (float)y + 90}, 24, 2, DARKGRAY);
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
int PetStats::GetSurvivalTime() { return survivalTime; }
float PetStats::GetHealth() { return health; }