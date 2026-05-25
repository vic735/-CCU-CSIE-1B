//角色實作

#include "VirtualPet.h"
#include "raymath.h"//向量工程數學 Raylib 提供的數學函式庫
#include <iostream>

//建構子實作
VirtualPet::VirtualPet(std::string petName , const char* imagePath , float startX, float startY){
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

}

//建構子實作
VirtualPet::~VirtualPet(){
    // RAII 原則：物件消滅時把圖片資源釋放，不佔用記憶體
    UnloadTexture(texture);
}

//邏輯更新實作
void VirtualPet::Update(){

    stats.Update(); //背包
    
    if(!isFollowing){
        wanderTimer -= GetFrameTime(); //減去每幀 by AI
        if (wanderTimer <= 0){
            //決定下一個位置
            targetPos.x=(float)GetRandomValue(100 , 700);
            targetPos.y=(float)GetRandomValue(100 , 500);
            wanderTimer = (float)GetRandomValue(2,5);//隨機2~5秒換地方
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
void VirtualPet::Draw(){

    DrawTextureEx(texture, Vector2{x,y}, 0.0f, 0.2f, WHITE);
    //語法DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);
    //texture圖片   Vector2 position用二為向量把x,y裝在一起   rotation跟scale選轉角度跟縮放比例  
    //呼叫Raylib指定圖片位置(x,y) WHITE表示使用原色不調色 DrawTexture(texture, (int)x , (int)y , WHITE); 改掉了
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

void VirtualPet::Feed() {
    stats.AddHunger(30.0f); // 吃到飼料，恢復 30 點飢餓
    stats.AddMood(10.0f);   // 恢復 10 點心情
}

PetStats* VirtualPet::GetStats() {
    return &stats; // 把背包借給外面畫圖
}

float VirtualPet::GetX() { return x; }
float VirtualPet::GetY() { return y; }