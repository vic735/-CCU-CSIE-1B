//視窗實作

#include "GameWindow.h"

//視窗建構子實作
GameWindow::GameWindow(int w,int h , const char* t){
    width = w;
    height = h;
    title = t ;

    //初始化視窗
    InitWindow(width , height , title);
    SetTargetFPS(60); //固定幀數 應該吧

    //載入中文
    const char* testText = u8"是一隻寵物"; //固定UTF-8

    int codepointCount = 0;//宣告變數紀錄需要字數
    int *codepoints = LoadCodepoints(testText, &codepointCount);//Raylib內建函數 掃描testText詞彙表，轉換為編碼數字存入codepointCount

    chineseFont = LoadFontEx("GenSenRounded2TW-B.otf", 32, codepoints, codepointCount);//抓資料夾內的字 並設定大小 最後變成圖片匯出 壓縮資源
    UnloadCodepoints(codepoints);//刪除舊的資料確保使用空間

    //載入寵物
    myPet = new VirtualPet("寵物名","pet.png",300,200);

    // 為什麼不在 .h 檔直接寫 `VirtualPet myPet;`？
    // 因為 Raylib 規定：【必須先 InitWindow 開啟顯示卡環境，才能 LoadTexture 載入圖片】。
    // 所以我們必須在這裡（視窗開好之後），才把寵物「生」出來。 by-AI
}

//視窗解構子實作
GameWindow::~GameWindow(){
    delete myPet;//這會觸發VirtualPet 的解構子，清掉圖片記憶體

    UnloadFont(chineseFont);
    CloseWindow();
    //因為適用new建立的實體，因此結束時必須手動銷毀 
}

void GameWindow::Run(){

    while(!WindowShouldClose()){

        // 邏輯更新
        myPet -> Update(); //寵物呼叫狀態 指標呼叫原來是用-> 好酷喔

        //畫面渲染
        BeginDrawing();
        ClearBackground(RAYWHITE);

        //畫出寵物
        myPet -> Draw();

        //畫出中文對白
        DrawTextEx(chineseFont, u8"是一隻寵物",Vector2{ 200, 400 }, 32, 2, DARKGRAY);

        EndDrawing();
    }
}