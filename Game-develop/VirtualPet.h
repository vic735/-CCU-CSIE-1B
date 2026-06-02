//角色設定

#pragma once
#include "raylib.h"
#include <string>
#include "PetStats.h"

class VirtualPet{
    private:
        std::string name;
        float x; //座標
        float y; //同上
        Texture2D texture; //存放畫作變數
        Texture2D textureHurt;

        float speed; //移動速度
        Vector2 targetPos; //目標位置
        bool isFollowing; //是否更隨玩家
        float wanderTimer; //閒晃計時器

        PetStats stats; //引入狀態系統

        std::string currentSpeech; // ⚠️ 新增：目前要說的話
        float speechTimer;         // ⚠️ 新增：說話計時器

    public:
    //建構子 用來誕生寵物
    VirtualPet(std::string petName, const char* imagePath, const char* hurtImagePath, float startX, float startY);

    //解構子 消失寵物
    ~VirtualPet();

    //行為方法
    void Update(); //邏輯更新
    void Draw(Font font);  //畫面繪製

    void SetTarget(Vector2 pos, bool follow); //提供介面讓外部設定跟隨狀態
    //Vector2 pos目標位置 包含x,y
    //bool follow 跟隨狀態

    std::string GetName();
    void SetName(std::string newNAME);

    void Feed();
    PetStats* GetStats();
    float GetX();
    float GetY();
    
    void Speak(std::string text, float duration);

};