//視窗實作.

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

    currentState = STATE_NAMING; 
    inputText = ""; 
    hasUploaded = false;

    currentState = STATE_NAMING; //初始化命名畫面
    inputText = ""; //輸入文字，預設空白
    framesCounter = 0;
    enemySpawnTimer = 0.0f; //初始化敵人生出計時器


    //初始化視窗
    InitWindow(width , height , title);
    SetTargetFPS(60); //固定幀數 應該吧

    //載入中文
    const char* dictionary = u8"飼料寵物正在跟著你！是一隻，我叫做請為取個名字：按鍵確認Enter大雞雞波波改名輸入英文數飢餓度心情值存活時間秒血量遊戲結束重新開始最終今天天氣真好想散步你在看我嗎好吃我快不行了痛";

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
    myPet = new VirtualPet("寵物名","pet.png","pet_hurt.png",300,200);

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

            //偵測是否有點飼料
            if(CheckCollisionPointRec(mousePos , foodBtn)){
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    isFoodActive = !isFoodActive; //切換飼料狀態
                }
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

            //敵人
            enemySpawnTimer += GetFrameTime();
            if (enemySpawnTimer > 1.5f) { // 每 1.5 秒隨機生出一隻
                enemySpawnTimer = 0.0f;
                Enemy newEnemy;
                newEnemy.radius = 15.0f;
                
                int edge = GetRandomValue(0, 3); // 決定要在哪一邊生成 (0上 1右 2下 3左)
                if (edge == 0) { 
                    newEnemy.x = GetRandomValue(0, width); newEnemy.y = -50; 
                    newEnemy.speedX = GetRandomValue(-3, 3); newEnemy.speedY = GetRandomValue(2, 6); 
                } else if (edge == 1) { 
                    newEnemy.x = width + 50; newEnemy.y = GetRandomValue(0, height); 
                    newEnemy.speedX = GetRandomValue(-6, -2); newEnemy.speedY = GetRandomValue(-3, 3); 
                } else if (edge == 2) { 
                    newEnemy.x = GetRandomValue(0, width); newEnemy.y = height + 50; 
                    newEnemy.speedX = GetRandomValue(-3, 3); newEnemy.speedY = GetRandomValue(-6, -2); 
                } else { 
                    newEnemy.x = -50; newEnemy.y = GetRandomValue(0, height); 
                    newEnemy.speedX = GetRandomValue(2, 6); newEnemy.speedY = GetRandomValue(-3, 3); 
                }
                enemies.push_back(newEnemy); // 把這隻新敵人丟進陣列裡
            }

            // 敵人的移動與碰撞判定
            for (auto it = enemies.begin(); it != enemies.end(); ) {
                it->x += it->speedX;
                it->y += it->speedY;

                // 判斷是否飛出畫面外
                bool hitBound = (it->x < -100 || it->x > width + 100 || it->y < -100 || it->y > height + 100);

                // 計算寵物(中心點)與這隻敵人的距離
                float dist = Vector2Distance(Vector2{myPet->GetX() + 50, myPet->GetY() + 50}, Vector2{it->x, it->y});
                bool hitPet = (dist < it->radius + 40.0f); // 碰撞半徑

                if (hitPet) {
                    myPet->GetStats()->TakeDamage(20.0f); 
                    myPet->Speak(u8"好痛！", 1.0f); // ⚠️ 撞到時發出哀嚎
                }
                

                // 如果飛出界外，或是撞到寵物，就把這隻敵人從陣列中刪除
                if (hitBound || hitPet) {
                    it = enemies.erase(it); 
                } else {
                    ++it;
                }
            }

            // 邏輯更新
            myPet -> Update(); //寵物呼叫狀態 指標呼叫原來是用-> 好酷喔

            //死亡判定
                    if (myPet->GetStats()->GetHealth() <= 0) {
                            currentState = STATE_GAMEOVER;

                            if (!hasUploaded) {
                    std::string playerName = myPet->GetName();
                    int survivalSec = myPet->GetStats()->GetSurvivalTime();


                    // 2. 把玩家名字與存活時間打包成簡單的 JSON 字串
                    std::string jsonBody = "{\"player\": \"" + playerName + "\", \"score\": " + std::to_string(survivalSec) + "}";

                    hasUploaded = true; // 鎖上開關，這一局不再重複上傳！
                }
            }

            //畫面渲染
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // 畫出所有敵人 (紫色的圓球)
            for (const auto& enemy : enemies) {
                DrawCircleV(Vector2{enemy.x, enemy.y}, enemy.radius, PURPLE);
            }
             //畫出寵物
            myPet -> Draw(chineseFont);

            //劃出數值

            myPet -> GetStats()->DrawUI(30 , 480 , chineseFont);

            //繪製按鈕
            DrawRectangleRec(foodBtn, isFoodActive ? GOLD : LIGHTGRAY); //按鈕顏色與狀態
            DrawRectangleLinesEx(foodBtn , 2 , DARKGRAY);
            DrawTextEx(chineseFont , u8"飼料", Vector2{ foodBtn.x + 15, foodBtn.y + 5 }, 24, 2, BLACK);

            //繪製名稱按鈕
            DrawRectangleRec(renameBth, LIGHTGRAY);
            DrawRectangleLinesEx(renameBth, 2, DARKGRAY); 
            DrawTextEx(chineseFont, u8"改名", Vector2{ renameBth.x + 15, renameBth.y + 5 }, 24, 2, BLACK);


            //在滑鼠位置繪製一顆飼料
            if (isFoodActive){
                DrawCircleV(mousePos , 10 , ORANGE);
                //DrawTextEx(chineseFont,u8"他跟著你", Vector2{150 , 25},24,2 ,DARKGRAY);
            }

            EndDrawing();
        }

        //遊戲結束畫面
        else if (currentState == STATE_GAMEOVER) {
            
            // 偵測是否按下 Enter 重新開始
            if (IsKeyPressed(KEY_ENTER)) {
                delete myPet; // 殺掉舊的寵物，釋放記憶體
                myPet = new VirtualPet("寵物名", "pet.png", "pet_hurt.png", 300, 200); // 誕生一隻全新的寵物，背包狀態會自動重置！
                
                enemies.clear(); // 清空舊的敵人陣列
                enemySpawnTimer = 0.0f;

                currentState = STATE_NAMING; // 回到取名畫面重新開始
                inputText = ""; 
                isFoodActive = false;

                hasUploaded = false;
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);

            // 印出死亡訊息與最終存活時間
            DrawTextEx(chineseFont, u8"遊戲結束 (GAME OVER)", Vector2{ 200, 200 }, 40, 2, RED);
            
            std::string finalTime = u8"最終存活時間: " + std::to_string(myPet->GetStats()->GetSurvivalTime()) + u8" 秒";
            DrawTextEx(chineseFont, finalTime.c_str(), Vector2{ 200, 280 }, 32, 2, DARKGRAY);

            DrawTextEx(chineseFont, u8"按 Enter 鍵重新開始", Vector2{ 200, 360 }, 32, 2, GRAY);

            EndDrawing();
        }
        
    }
}