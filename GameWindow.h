//視窗設定

#pragma once 
#include "raylib.h"
#include "VirtualPet.h"
#include <string>


//命名定義 正式 與 遊玩
enum GameState{
    STATE_NAMING,
    STATE_PLAYING
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
        Rectangle clickGoldBtn; // 新增：賺錢點擊區
        Rectangle buyFoodBtn;   // 新增：買飼料按鈕

        Rectangle renameBth;      //名稱按鈕
        GameState currentState;   //目前的狀態
        std::string inputText;    //輸入文字
        int framesCounter;        //幀數計時器

    public:
        GameWindow(int w, int h , const char* t);
        ~GameWindow();

        void Run();
};