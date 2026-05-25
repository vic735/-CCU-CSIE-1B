//數值管理實作

#include "PetStats.h"

PetStats::PetStats(){
    hunger = 50.0f;
    mood = 80.0f;
    timet = 0.0f;
}

PetStats::~PetStats(){

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

void PetStats::DrawUI(int x, int y, Font font){
    //統一解釋，劃出飢餓度跟心情 反正就是讓數值UI化

    DrawTextEx(font, u8"飢餓度:", Vector2{(float)x, (float)y}, 24, 2, DARKGRAY);
    DrawRectangle(x + 100, y + 5, 100, 15, LIGHTGRAY); // 100 是滿血長度
    DrawRectangle(x + 100, y + 5, (int)hunger, 15, ORANGE); 

    DrawTextEx(font, u8"心情值:", Vector2{(float)x, (float)y + 30}, 24, 2, DARKGRAY);
    DrawRectangle(x + 100, y + 35, 100, 15, LIGHTGRAY);
    DrawRectangle(x + 100, y + 35, (int)mood, 15, PINK);
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