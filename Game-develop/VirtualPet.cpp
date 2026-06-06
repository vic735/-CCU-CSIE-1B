//角色實作

#include "VirtualPet.h"
#include "raymath.h"//向量工程數學 Raylib 提供的數學函式庫
#include <iostream>

//建構子實作
VirtualPet::VirtualPet(std::string petName , const char* imagePath , const char* hurtImagePath, float startX, float startY){
        name = petName ;
        //hunger = 50 ; 
        //初始飢餓值 坐在標頭擋了
        x=startX; //初始位置
        y=startY;

        speed = 2.0f; //走路速度
        isFollowing = false; //跟隨狀態
        targetPos = {x,y}; //滑鼠位置
        wanderTimer = 0; //我不知道，有空再研究 好像是幀數歸零

        texture = LoadTexture(imagePath); //在顯卡記憶體仔入圖片;
        textureHurt = LoadTexture(hurtImagePath); //受傷圖片

}

//建構子實作
VirtualPet::~VirtualPet(){
    // RAII 原則：物件消滅時把圖片資源釋放，不佔用記憶體
    UnloadTexture(texture);
    UnloadTexture(textureHurt);
}

//邏輯更新實作
void VirtualPet::Update(){

    stats.Update(isFollowing); //背包

    if (stats.GetHunger() < 30.0f || stats.GetMood() < 30.0f || stats.GetHealth() < 30.0f) {
        speed = 0.5f; 
        if (speechTimer <= 0) {
            Speak(u8"我快不行了...", 2.0f);
        }
    } else {
        speed = 2.0f; 
    }

    if (speechTimer > 0) {
        speechTimer -= GetFrameTime();
        if (speechTimer <= 0) {
            currentSpeech = ""; 
        }
    }
    
    if(!isFollowing){
        wanderTimer -= GetFrameTime(); //減去每幀 by AI
        if (wanderTimer <= 0){
            //決定下一個位置
            targetPos.x=(float)GetRandomValue(100 , 700);
            targetPos.y=(float)GetRandomValue(100 , 500);
            wanderTimer = (float)GetRandomValue(2,5);//隨機2~5秒換地方

            if (stats.GetHealth() >= 30.0f && currentSpeech == "" && GetRandomValue(1, 100) > 60) {
                const char* randomChats[] = { u8"今天天氣真好", u8"想散步...", u8"你在看我嗎？" };
                Speak(randomChats[GetRandomValue(0, 2)], 2.5f);
            }

            }
        }
        Vector2 currentPos = {x,y};//朝目標移動

        float distance = Vector2Distance(currentPos, targetPos);//計算目標與現在位置

        if(distance > 5.0f){ //如果還沒走到就繼續走
            Vector2 direction = Vector2Normalize(Vector2Subtract(targetPos, currentPos));//計算移動方向 向量減法並單位化

            x += direction.x * speed;
            y += direction.y * speed;
            //更新位置
    }
}

//畫面實作
void VirtualPet::Draw(Font font, bool enableGrowth){

    float growthScale = 1.0f;
    if (enableGrowth) {
        growthScale = 1.0f + (stats.GetSurvivalTime() / 30.0f) * 0.1f;
        if (growthScale > 2.5f) growthScale = 2.5f;
    }
    float finalScale = 0.2f * growthScale;

    // 1. 視覺差分：判斷要畫哪一張圖
    if (stats.GetHealth() < 30.0f || stats.GetHunger() < 30.0f) {
        DrawTextureEx(textureHurt, Vector2{x, y}, 0.0f, 0.2f, WHITE);
    } else {
        DrawTextureEx(texture, Vector2{x, y}, 0.0f, finalScale, WHITE);
    }

    // 2. 畫出對白氣泡框
    if (speechTimer > 0 && currentSpeech != "") {
        int textWidth = MeasureTextEx(font, currentSpeech.c_str(), 24, 2).x;
        
        // 對話框背景 (半透明白底圓角矩形)
        DrawRectangleRounded(Rectangle{x + 50 - (textWidth/2.0f) - 10, y - 45, (float)textWidth + 20, 35}, 0.5f, 10, Fade(WHITE, 0.9f));
        DrawRectangleRoundedLines(Rectangle{x + 50 - (textWidth/2.0f) - 10, y - 45, (float)textWidth + 20, 35}, 0.5f, 10, GRAY);
        
        // 對白文字
        DrawTextEx(font, currentSpeech.c_str(), Vector2{x + 50 - (textWidth/2.0f), y - 40}, 24, 2, BLACK);
    }
}

//跟著滑鼠動
void VirtualPet::SetTarget(Vector2 pos , bool follow){
    isFollowing = follow ;

    if (isFollowing){//只在有飼料時跟隨
        targetPos = pos;
    }
}

// 實作名字 Getter 與 Setter
std::string VirtualPet::GetName() {
    return name;
}

void VirtualPet::SetName(std::string newName) {
    name = newName;
}

void VirtualPet::Feed(int foodType) {
    stats.EatSpecialFood(foodType);

    if (foodType == 1) {
        Speak(u8"爽啦！但肚子好空... (咳血)", 2.5f);
    } 
    else if (foodType == 2) {
        Speak(u8"好飽！但這什麼地獄味道... (胃痛)", 2.5f);
    } 
    else if (foodType == 3) {
        Speak(u8"傷口癒合了！但我覺得身體好虛...", 2.5f);
    }
    else if (foodType == 4) {
        Speak(u8"好吃！嚼嚼嚼...", 1.5f);
    }
}

PetStats* VirtualPet::GetStats() {
    return &stats; // 把背包借給外面畫圖
}

float VirtualPet::GetX() { return x; }
float VirtualPet::GetY() { return y; }

void VirtualPet::Speak(std::string text, float duration) {
    currentSpeech = text;
    speechTimer = duration;
}