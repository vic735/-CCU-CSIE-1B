//視窗實作

#include "GameWindow.h"
#include "raymath.h"

//視窗建構子實作
GameWindow::GameWindow(int w,int h , const char* t){
    width = w;
    height = h;
    title = t ;
    isFoodActive = false; //檢測手上是否有飼料
    foodBtn ={20 , 20 , 100 , 40 };//按鈕位置
    renameBth = {130 , 20 , 100 , 40}; //名稱按鈕
    clickGoldBtn = {240, 20, 130, 40}; // 新增：賺錢點擊區
    buyFoodBtn = {380, 20, 150, 40};   // 新增：買飼料按鈕

    currentState = STATE_NAMING; //初始化命名畫面
    inputText = ""; //輸入文字，預設空白
    framesCounter = 0;


    //初始化視窗
    InitWindow(width , height , title);
    SetTargetFPS(60); //固定幀數 應該吧

    //載入中文
    const char* dictionary = u8"飼料寵物正在跟著你！是一隻，我叫做請為取個名字：按鍵確認Enter大雞雞波波改名輸入英文數飢餓度心情值"; 

    int codepointCount = 0;
    int *codepoints = LoadCodepoints(dictionary, &codepointCount);

    int totalCount = (127 - 32) + codepointCount; //建立矩陣，加入A~Z以及中文字典
    int* allCodepoints = new int[totalCount];
    int count = 0;

    for (int i = 32; i < 127; i++){
        allCodepoints[count++] = i;
    }

    for (int i = 0; i < codepointCount; i++){
        allCodepoints[count++] = codepoints[i];}
    


    // 精準載入字典裡有出現的字
    chineseFont = LoadFontEx("GenSenRounded2TW-B.otf", 32, allCodepoints, count);
    
    // 釋放名單記憶體
    UnloadCodepoints(codepoints);

    //載入寵物GameWindow.cpp
   myPet = new VirtualPet(std::string(u8"寵物名"), "pet.png", 300.0f, 200.0f);

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

        if(currentState == STATE_NAMING){  //命名功能

                int key = GetCharPressed(); //抓取玩家按下的案件的編碼

                while(key > 0){

                    if((key>=32) && (key<=125)  && (inputText.length()<12)){   //設定條件 只能有英文字母ASCII編碼 並且只能在12個字母內

                        inputText += (char)key; //把按下的字母加入字串內

                    }

                    key = GetCharPressed();  //繼續檢查有沒有同時按其他鍵
                }

                if (IsKeyPressed(KEY_BACKSPACE)){  //刪除字

                    if(inputText.length()>0){

                        inputText.pop_back();

                    }
                }


                if (IsKeyPressed(KEY_ENTER)){  //按下enter後 有輸入名字 就進入遊戲

                    if(inputText.length() > 0){  

                        myPet -> SetName(inputText); //寵物改名
                        currentState = STATE_PLAYING; //回到遊戲畫面
                    }
                }

        framesCounter++; //游標閃爍

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTextEx(chineseFont, u8"請為寵物取個名字：", Vector2{ 200, 200 }, 32, 2, DARKGRAY);

        //邊框設定
        DrawRectangle(200,250,400,50, LIGHTGRAY);
        DrawRectangleLines(200 , 250 , 400 , 50 , DARKGRAY);

        //印出字
        DrawTextEx(chineseFont, inputText.c_str(), Vector2{210 , 260}, 32 , 2 , BLACK);

        //閃爍游標 30幀閃爍一次
        if(((framesCounter / 30) %2 == 0) && inputText.length() < 12){
            //計算字串長度 讓游標跟在後面
            int textWidth = MeasureTextEx(chineseFont , inputText.c_str() , 32 , 2).x;
            DrawTextEx(chineseFont,"|", Vector2{215.0f + textWidth , 260.0f} , 32 , 2 , BLACK);
        }

        DrawTextEx(chineseFont , u8"輸入英文,enter確認", Vector2{200 , 320 } , 24 , 2 , GRAY);

        EndDrawing();
            }


        //遊戲畫面
    else if (currentState == STATE_PLAYING ){

        Vector2 mousePos = GetMousePosition();

        PetStats* stats = myPet->GetStats();//設定stats的導向

        // --- 新增 1：點擊賺錢偵測 ---
        if (CheckCollisionPointRec(mousePos, clickGoldBtn)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                stats->ClickForCoin(); // 點一下得10元
            }
        }

        // --- 新增 2：商店買飼料偵測 ($50元一包) ---
        if (CheckCollisionPointRec(mousePos, buyFoodBtn)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                stats->BuyFood(50); // 自動扣錢扣成功會加一個飼料
            }
        }

        // 修改原本點擊飼料的偵測：【必須庫存 > 0】才可以拿起飼料！
        if(CheckCollisionPointRec(mousePos , foodBtn)){
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                if (stats->GetFoodCount() > 0) {
                    isFoodActive = !isFoodActive; 
                }
            }
        }
        
        // 如果手上有飼料但庫存突然歸零（被吃掉），自動取消手持狀態
        if (isFoodActive && stats->GetFoodCount() <= 0) {
            isFoodActive = false;
        }
        
        //偵測改名按鈕
        if(CheckCollisionPointRec(mousePos , renameBth)){
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                currentState = STATE_NAMING; //切換回命名葉面
                inputText = myPet -> GetName();
                isFoodActive = false;
            }
        }

        //強制改變動物目標位置
        if(isFoodActive){
            myPet -> SetTarget(Vector2{mousePos.x-50 , mousePos.y-50} , true);//稍微修正位置

            float dist = Vector2Distance(Vector2{myPet -> GetX() + 50 , myPet -> GetY() + 50},mousePos);

            //如果距離夠近 (小於40像素)，代表大雞雞碰到飼料了！ by AI
            if(dist < 40.0f){
                myPet -> Feed(); //呼叫餵食
                isFoodActive = false; //手上的飼料被吃掉 狀態取消
            }

        }
        else{
            myPet -> SetTarget(mousePos,false);
        }

        //畫面渲染
        BeginDrawing();
        ClearBackground(RAYWHITE);

        //畫出寵物
        myPet -> Draw();

        //劃出數值
        myPet -> GetStats()->DrawUI(30 , 480 , chineseFont);


        // 邏輯更新
        myPet -> Update(); //寵物呼叫狀態 指標呼叫原來是用-> 好酷喔

        //繪製按鈕
        DrawRectangleRec(foodBtn, isFoodActive ? GOLD : LIGHTGRAY); //按鈕顏色與狀態
        DrawRectangleLinesEx(foodBtn , 2 , DARKGRAY);
        DrawTextEx(chineseFont , u8"飼料", Vector2{ foodBtn.x + 15, foodBtn.y + 5 }, 24, 2, BLACK);

        //繪製名稱按鈕
        DrawRectangleRec(renameBth, LIGHTGRAY);
        DrawRectangleLinesEx(renameBth, 2, DARKGRAY); 
        DrawTextEx(chineseFont, u8"改名", Vector2{ renameBth.x + 15, renameBth.y + 5 }, 24, 2, BLACK);

        // --- 新增 3：渲染點擊賺錢按鈕 ---
        DrawRectangleRec(clickGoldBtn, SKYBLUE);
        DrawRectangleLinesEx(clickGoldBtn, 2, DARKGRAY);
        DrawTextEx(chineseFont, u8"點擊賺錢", Vector2{ clickGoldBtn.x + 15, clickGoldBtn.y + 5 }, 24, 2, BLACK);

        // --- 新增 4：渲染買飼料按鈕 ---
        DrawRectangleRec(buyFoodBtn, GREEN);
        DrawRectangleLinesEx(buyFoodBtn, 2, DARKGRAY);
        DrawTextEx(chineseFont, u8"買飼料$50", Vector2{ buyFoodBtn.x + 15, buyFoodBtn.y + 5 }, 24, 2, WHITE);

        //在滑鼠位置繪製一顆飼料
        if (isFoodActive){
            DrawCircleV(mousePos , 10 , ORANGE);
            //DrawTextEx(chineseFont,u8"他跟著你", Vector2{150 , 25},24,2 ,DARKGRAY);

        }

        EndDrawing();
            
        }
        
    }
}