//視窗設定

#pragma once 
#include "raylib.h"
#include "VirtualPet.h"
#include <string>
#include <vector> //動態函式庫用來記錄敵人
#include "Shop.h"
#include "UpgradeShop.h"
#include "RewardManager.h"


//命名定義 正式 與 遊玩
enum GameState{
    STATE_MENU,
    STATE_NAMING,
    STATE_PLAYING,
    STATE_GAMEOVER,
    STATE_SHOP,
    STATE_UPGRADE_SHOP,
    STATE_RAISING, //養成模式
    STATE_RAISING_NAMING
};

// 敵人的資料結構
struct Enemy {
    float x;
    float y;
    float speedX;
    float speedY;
    float radius;
    bool isHoming;
    float lifeTimer;
};

class GameWindow{

    private:
        int width;
        int height;
        const char* title;

        Font chineseFont;

        VirtualPet* myPet;//用指標來持有寵物物件
        VirtualPet* myRaisingPet;

        int activeFoodType; //手上是否有飼料
        Rectangle basicFoodBtn;
        Rectangle jellyBtn;
        Rectangle jerkyBtn;
        Rectangle pillBtn;

        Rectangle buyFoodBtn;   // 新增：買飼料按鈕 //商店的按鈕
        Rectangle closeShopBtn; // 離開商店的按鈕
        GameState previousState;  //判斷當前位置

        Rectangle renameBth;      //名稱按鈕
        GameState currentState;   //目前的狀態
        std::string inputText;    //輸入文字
        int framesCounter;        //幀數計時器

        // 管理敵人的變數
        std::vector<Enemy> enemies; // 裝敵人的動態陣列 
        float enemySpawnTimer;      // 控制多久生一隻敵人的計時器

        bool hasUploaded;//網路
        Shop* myShop;
        UpgradeShop* myUpgradeShop;

        RenderTexture2D targetCanvas;

        RewardManager* myReward;
        
        Rectangle startMenuBtn;   
        Rectangle futureModeBtn;
        Rectangle backToMenuBtn;

        void SaveGame();
        void LoadGame();

    public:
        GameWindow(int w, int h , const char* t);
        ~GameWindow();

        void Run();

};