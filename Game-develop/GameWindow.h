//視窗設定

#pragma once 
#include "raylib.h"
#include "VirtualPet.h"
#include <string>
#include <vector> //動態函式庫用來記錄敵人


//命名定義 正式 與 遊玩
enum GameState{
    STATE_NAMING,
    STATE_PLAYING,
    STATE_GAMEOVER
};

// 敵人的資料結構
struct Enemy {
    float x;
    float y;
    float speedX;
    float speedY;
    float radius;
};

class GameWindow{

    private:
        int width;
        int height;
        const char* title;

        Font chineseFont;

        VirtualPet* myPet;//用指標來持有寵物物件

        bool isFoodActive;  //手上是否有飼料
        Rectangle foodBtn;  //飼料按鈕的區域

        Rectangle renameBth;      //名稱按鈕
        GameState currentState;   //目前的狀態
        std::string inputText;    //輸入文字
        int framesCounter;        //幀數計時器

        // 管理敵人的變數
        std::vector<Enemy> enemies; // 裝敵人的動態陣列 
        float enemySpawnTimer;      // 控制多久生一隻敵人的計時器

        bool hasUploaded;//網路

    public:
        GameWindow(int w, int h , const char* t);
        ~GameWindow();

        void Run();
};