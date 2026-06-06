//視窗實作

#include "GameWindow.h"
#include "raymath.h"
#include <fstream>
#include "json.hpp"


//視窗建構子實作
GameWindow::GameWindow(int w,int h , const char* t){
    width = w;
    height = h;
    title = t ;
    activeFoodType = 0; //檢測手上是否有飼料
    basicFoodBtn = { 20, 20, 100, 40 };//按鈕位置
    jellyBtn     = { 130, 20, 110, 40 };
    jerkyBtn     = { 250, 20, 110, 40 };
    pillBtn      = { 370, 20, 110, 40 };
    buyFoodBtn   = { 490, 20, 110, 40 };

    renameBth = {610 , 20 , 100 , 40}; //名稱按鈕

    //currentState = STATE_MENU; //選單畫面
    startMenuBtn  = { 330, 220, 300, 80 }; 
    futureModeBtn = { 330, 320, 300, 80 };

    backToMenuBtn = { 20, 480, 130, 40 };

    hasUploaded = false;

    currentState = STATE_MENU; //初始化主畫面
    inputText = ""; //輸入文字，預設空白
    framesCounter = 0;
    enemySpawnTimer = 0.0f; //初始化敵人生出計時器


    //初始化視窗
    InitWindow(width , height , title);
    SetTargetFPS(60); //固定幀數 應該吧

    //載入中文
    const char* dictionary = u8"爽啦！但肚子好空... (咳血) 好飽！但這什麼礎地獄味道養雞場.=== 電子開新模式 (主選單未開沒錢買基礎飼料了...放)始遊戲雞世界 ===.. (胃痛)傷口癒合了！但我覺得身體好虛...好吃！嚼嚼嚼...魔極樂果凍肉乾精全滿飽食本煉大補丸包打開或 的按 U 鍵 進入基因研究院永久升級永久當前生命上限:改造生命 (+20) 消耗$100飼料寵物正在跟著你！是一隻，我叫飼料庫存做請為取個名字：按鍵確認Ente擁有金幣包r大雞雞波波改打離開研究院永久升級最大基因能力改造消耗點返回並重開局按鍵進入名輸入英文數飢餓度心歷史最高紀錄情值存活時間秒血量遊戲結束重新開始最終今天天氣真好想散步你在看我嗎好吃我快不行了痛點擊賺錢買商店離開購買黃金飼料";

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
    chineseFont = LoadFontEx("GenSenRounded2TW-B.otf",128, allCodepoints, count);
    
    // 釋放名單記憶體
    UnloadCodepoints(codepoints);

    //載入寵物GameWindow.cpp
    myPet = new VirtualPet("寵物名","pet.png","pet_hurt.png",300,200);
    myRaisingPet = new VirtualPet("養雞場小雞","pet.png","pet_hurt.png",300,200);

    // 為什麼不在 .h 檔直接寫 `VirtualPet myPet;`？
    // 因為 Raylib 規定：【必須先 InitWindow 開啟顯示卡環境，才能 LoadTexture 載入圖片】。
    // 所以我們必須在這裡（視窗開好之後），才把寵物「生」出來。 by-AI

    myShop = new Shop();//建立商店實體
    myUpgradeShop = new UpgradeShop();
    
    targetCanvas = LoadRenderTexture(960, 540);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTextureFilter(targetCanvas.texture, TEXTURE_FILTER_BILINEAR);
    myReward = new RewardManager(2);
    LoadGame();

}

//視窗解構子實作
GameWindow::~GameWindow(){
    delete myPet;//這會觸發VirtualPet 的解構子，清掉圖片記憶體
    delete myRaisingPet;

    UnloadFont(chineseFont);
    CloseWindow();
    //因為適用new建立的實體，因此結束時必須手動銷毀 
    delete myShop;
    delete myUpgradeShop;
    UnloadRenderTexture(targetCanvas);
    delete myReward;
}

void GameWindow::Run(){

    while(!WindowShouldClose()){
        Vector2 mousePos = GetMousePosition();
        mousePos.x = mousePos.x * (960.0f / GetScreenWidth());
        mousePos.y = mousePos.y * (540.0f / GetScreenHeight());

        
        /*BeginDrawing();
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
        }*/

        BeginTextureMode(targetCanvas); 
        ClearBackground(RAYWHITE);


        if (currentState == STATE_MENU) {  //主選單

            if (CheckCollisionPointRec(mousePos, startMenuBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentState = STATE_NAMING;
            }
            if (CheckCollisionPointRec(mousePos, futureModeBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                // 💡 檢查養雞場小雞的名字是否等於建構子給的預設名字 "養雞場小雞"
                if (myRaisingPet->GetName() == "養雞場小雞") {
                    currentState = STATE_RAISING_NAMING; // 還是預設名，代表是第一次，去命名畫面
                    inputText = ""; 
                } else {
                    currentState = STATE_RAISING;        // 名字已經被改過了，直接進養雞場放鬆！
                }
            }
            
            DrawTextEx(chineseFont, u8"=== 電子雞世界 ===", Vector2{ 210, 100 }, 54, 2, GOLD);

            // 繪製「開始遊戲」按鈕
            DrawRectangleRec(startMenuBtn, SKYBLUE);
            DrawRectangleLinesEx(startMenuBtn, 2, BLACK);
            DrawTextEx(chineseFont, u8"開始遊戲", Vector2{ startMenuBtn.x + 60, startMenuBtn.y + 20 }, 44, 1, LIGHTGRAY);

            // 預留的「新模式」灰色按鈕
            DrawRectangleRec(futureModeBtn, LIME);
            DrawRectangleLinesEx(futureModeBtn, 2, BLACK);
            DrawTextEx(chineseFont, u8"養雞場", Vector2{ futureModeBtn.x + 20, futureModeBtn.y + 20 }, 35, 1, LIGHTGRAY);

            if (CheckCollisionPointRec(mousePos, startMenuBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentState = STATE_NAMING;
            }
            
        }

        else if(currentState == STATE_NAMING){  //命名功能

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

        DrawTextEx(chineseFont, u8"請為寵物取個名字：", Vector2{ 280, 200 }, 32, 2, DARKGRAY);

        //邊框設定
        DrawRectangle(280,250,400,50, LIGHTGRAY);
        DrawRectangleLines(280 , 250 , 400 , 50 , DARKGRAY);

        //印出字
        DrawTextEx(chineseFont, inputText.c_str(), Vector2{290 , 260}, 32 , 2 , BLACK);

        //閃爍游標 30幀閃爍一次
        if(((framesCounter / 30) %2 == 0) && inputText.length() < 12){
            //計算字串長度 讓游標跟在後面
            int textWidth = MeasureTextEx(chineseFont , inputText.c_str() , 32 , 2).x;
            DrawTextEx(chineseFont,"|", Vector2{295.0f + textWidth , 260.0f} , 32 , 2 , BLACK);
        }

        DrawTextEx(chineseFont , u8"輸入英文,enter確認", Vector2{280 , 320 } , 24 , 2 , GRAY);

        }
        else if(currentState == STATE_RAISING_NAMING){  //命名功能

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

                        myRaisingPet -> SetName(inputText); //寵物改名
                        currentState = STATE_RAISING; //回到遊戲畫面
                    }
                }

        framesCounter++; //游標閃爍

        DrawTextEx(chineseFont, u8"請為寵物取個名字：", Vector2{ 280, 200 }, 32, 2, DARKGRAY);

        //邊框設定
        DrawRectangle(280,250,400,50, LIGHTGRAY);
        DrawRectangleLines(280 , 250 , 400 , 50 , DARKGRAY);

        //印出字
        DrawTextEx(chineseFont, inputText.c_str(), Vector2{290 , 260}, 32 , 2 , BLACK);

        //閃爍游標 30幀閃爍一次
        if(((framesCounter / 30) %2 == 0) && inputText.length() < 12){
            //計算字串長度 讓游標跟在後面
            int textWidth = MeasureTextEx(chineseFont , inputText.c_str() , 32 , 2).x;
            DrawTextEx(chineseFont,"|", Vector2{295.0f + textWidth , 260.0f} , 32 , 2 , BLACK);
        }

        DrawTextEx(chineseFont , u8"輸入英文,enter確認", Vector2{280 , 320 } , 24 , 2 , GRAY);

        }




        //遊戲畫面
        else if (currentState == STATE_PLAYING ){

            PetStats* stats = myPet->GetStats();
            myPet->Draw(chineseFont);

            // 點擊基礎飼料 (無限使用，點一下裝填，再點一下收回)
        if (CheckCollisionPointRec(mousePos, basicFoodBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            activeFoodType = (activeFoodType == 4) ? 0 : 4;
        }
        // 點擊果凍 (需庫存 > 0)
        if (CheckCollisionPointRec(mousePos, jellyBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (stats->GetJellyCount() > 0) activeFoodType = (activeFoodType == 1) ? 0 : 1;
        }
        // 點擊肉乾 (需庫存 > 0)
        if (CheckCollisionPointRec(mousePos, jerkyBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (stats->GetJerkyCount() > 0) activeFoodType = (activeFoodType == 2) ? 0 : 2;
        }
        // 點擊大補丸 (需庫存 > 0)
        if (CheckCollisionPointRec(mousePos, pillBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (stats->GetPillCount() > 0) activeFoodType = (activeFoodType == 3) ? 0 : 3;
        }


        // --- 新增 2：商店買飼料偵測 ($50元一包) ---
        if (CheckCollisionPointRec(mousePos, buyFoodBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            previousState = STATE_PLAYING;
            currentState = STATE_SHOP;
        }

            
            //偵測改名按鈕
            if(CheckCollisionPointRec(mousePos , renameBth)){
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    currentState = STATE_NAMING; 
                    inputText = myPet -> GetName();
                    activeFoodType = 0;
                }
            }

            //強制改變動物目標位置
            if (activeFoodType > 0) {
                myPet->SetTarget(Vector2{mousePos.x - 50, mousePos.y - 50}, true);
                float dist = Vector2Distance(Vector2{myPet->GetX() + 50, myPet->GetY() + 50}, mousePos);

                if (dist < 40.0f) {
                    bool canEat = true;
                    if (activeFoodType != 4) {
                        canEat = stats->UseStock(activeFoodType); // 嘗試扣庫存
                    }

                    if (canEat) {
                        myPet->Feed(activeFoodType); // 呼叫寵物吃藥台詞與扣庫存
                    }
                    activeFoodType = 0; // 吃完放空滑鼠
                }
            } 
            else {
                myPet->SetTarget(mousePos, false);
            }

            //敵人
            enemySpawnTimer += GetFrameTime();
            int survivalSec = stats->GetSurvivalTime(); // 取得目前撐了多少秒
            float currentSpawnInterval = 3.0f - (survivalSec * 0.02f);
            if (currentSpawnInterval < 0.5f) currentSpawnInterval = 0.4f;

            // 隨時間增加敵人的基礎速度暴走值
            float speedBoost = survivalSec * 0.05f; 

            enemySpawnTimer += GetFrameTime();
            if (enemySpawnTimer > currentSpawnInterval) { //換成動態間隔
                enemySpawnTimer = 0.0f;
                Enemy newEnemy;
                newEnemy.radius = 15.0f;
                
                newEnemy.isHoming = (GetRandomValue(1, 100) <= 10);  //百分比判斷
                newEnemy.lifeTimer = 0.0f;

                int edge = GetRandomValue(0, 3); // 決定在哪一邊生成 (0上 1右 2下 3左)
                if (edge == 0) { 
                    newEnemy.x = GetRandomValue(0, width); newEnemy.y = -50; 
                    newEnemy.speedX = GetRandomValue(-3, 3); newEnemy.speedY = GetRandomValue(2, 6) + speedBoost; 
                } else if (edge == 1) { 
                    newEnemy.x = width + 50; newEnemy.y = GetRandomValue(0, height); 
                    newEnemy.speedX = GetRandomValue(-6, -2) - speedBoost; newEnemy.speedY = GetRandomValue(-3, 3); 
                } else if (edge == 2) { 
                    newEnemy.x = GetRandomValue(0, width); newEnemy.y = height + 50; 
                    newEnemy.speedX = GetRandomValue(-3, 3); newEnemy.speedY = GetRandomValue(-6, -2) - speedBoost; 
                } else { 
                    newEnemy.x = -50; newEnemy.y = GetRandomValue(0, height); 
                    newEnemy.speedX = GetRandomValue(2, 6) + speedBoost; newEnemy.speedY = GetRandomValue(-3, 3); 
                }
                enemies.push_back(newEnemy); // 把這隻新敵人丟進陣列裡
            }

            // 敵人的移動與碰撞判定
            for (auto it = enemies.begin(); it != enemies.end(); ) {

                if (it->isHoming) {  //判斷敵人類型
                    Vector2 petCenter = { myPet->GetX() + 50, myPet->GetY() + 50 }; // 大雞雞中心點
                    Vector2 enemyPos = { it->x, it->y };

                    it->lifeTimer += GetFrameTime();
                    
                    // 計算導彈到大雞雞的向量，並將其「正規化 (Normalize)」成單位長度 1 的方向
                    Vector2 direction = Vector2Subtract(petCenter, enemyPos);
                    direction = Vector2Normalize(direction);
                    
                    // 根據目前遊戲難度設定追蹤彈的速度大小
                    float homingSpeed = 3.0f + (stats->GetSurvivalTime() * 0.04f);
                    
                    // 即時修正速度方向，讓導彈像瘋狗一樣朝寵物刺過去！
                    //it->speedX = direction.x * homingSpeed;
                    //it->speedY = direction.y * homingSpeed;
                    float turnEase = 0.02f;

                    float targetSpeedX = direction.x * homingSpeed;
                    float targetSpeedY = direction.y * homingSpeed;
                    it->speedX = it->speedX + (targetSpeedX - it->speedX) * turnEase;
                    it->speedY = it->speedY + (targetSpeedY - it->speedY) * turnEase;
                    

                    
                }
                
                it->x += it->speedX;
                it->y += it->speedY;

                // 判斷是否飛出畫面外
                bool hitBound = (it->x < -100 || it->x > width + 100 || it->y < -100 || it->y > height + 100);

                // 計算寵物(中心點)與這隻敵人的距離
                float dist = Vector2Distance(Vector2{myPet->GetX() + 50, myPet->GetY() + 50}, Vector2{it->x, it->y});
                bool hitPet = (dist < it->radius + 40.0f); // 碰撞半徑

                if (hitPet) {
                    myPet->GetStats()->TakeDamage(20.0f); 
                    myPet->Speak(u8"好痛！", 1.0f); // 撞到時發出哀嚎
                }
                bool isExpired = (it->isHoming && it->lifeTimer > 3.0f);  //敵人最多存活時間

                // 如果飛出界外，或是撞到寵物，就把這隻敵人從陣列中刪除
                if (hitBound || hitPet || isExpired) {
                    it = enemies.erase(it); // 完美蒸發
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

                                int rewardCoins = myReward->CalculateEarnedCoins(survivalSec);
                                myPet->GetStats()->AddCoins(rewardCoins);
                                myReward->SaveSettlementJson("data.json", playerName, survivalSec, myPet->GetStats()->GetCoins());
                                hasUploaded = true;


                    // 2. 把玩家名字與存活時間打包成簡單的 JSON 字串

                    hasUploaded = true; // 鎖上開關，這一局不再重複上傳！
                }
            }

            //畫面渲染
            //BeginDrawing();
            //ClearBackground(RAYWHITE);

            // 畫出所有敵人 (紫色的圓球)
            for (const auto& enemy : enemies) {
                DrawCircleV(Vector2{enemy.x, enemy.y}, enemy.radius, enemy.isHoming ? RED : PURPLE);
            }
             //畫出寵物
            myPet -> Draw(chineseFont);

            //劃出數值

            myPet -> GetStats()->DrawUI(750 , 20 , chineseFont);

            //繪製名稱按鈕
            DrawRectangleRec(renameBth, LIGHTGRAY);
            DrawRectangleLinesEx(renameBth, 2, DARKGRAY); 
            DrawTextEx(chineseFont, u8"改名", Vector2{ renameBth.x + 15, renameBth.y + 5 }, 24, 2, BLACK);

        // --- 新增 4：渲染買飼料按鈕 ---
            //DrawRectangleRec(buyFoodBtn, GREEN);
           // DrawRectangleLinesEx(buyFoodBtn, 2, DARKGRAY);
          //  DrawTextEx(chineseFont, u8"打開商店", Vector2{ buyFoodBtn.x + 15, buyFoodBtn.y + 5 }, 24, 2, WHITE);


                        // 1. 基礎飼料 (選中變金色，沒選中變淺灰)
            DrawRectangleRec(basicFoodBtn, (activeFoodType == 4) ? GOLD : LIGHTGRAY);
            DrawTextEx(chineseFont, u8"基礎飼料", Vector2{ basicFoodBtn.x+10, basicFoodBtn.y+10 }, 18, 1, BLACK);
            if (activeFoodType == 4) {
                DrawRectangleLinesEx(basicFoodBtn, 3, BLACK); // 選中時加個黑邊強調
            }

           // 2. 果凍 (有存量顯現 PINK，沒存量褪色為 GRAY)
            std::string txt1 = u8"果凍 x" + std::to_string(stats->GetJellyCount());
            DrawRectangleRec(jellyBtn, (stats->GetJellyCount() > 0) ? PINK : GRAY); // ⚠️ 依庫存決定顏色
            DrawTextEx(chineseFont, txt1.c_str(), Vector2{ jellyBtn.x + 10, jellyBtn.y + 10 }, 18, 1, WHITE);
            if (activeFoodType == 1) {
                DrawRectangleLinesEx(jellyBtn, 3, GOLD); // ⚠️ 裝填中：套上高亮金邊！
            }

            // 3. 肉乾 (有存量顯現 ORANGE，沒存量褪色為 GRAY)
            std::string txt2 = u8"肉乾 x" + std::to_string(stats->GetJerkyCount());
            DrawRectangleRec(jerkyBtn, (stats->GetJerkyCount() > 0) ? ORANGE : GRAY); // ⚠️ 依庫存決定顏色
            DrawTextEx(chineseFont, txt2.c_str(), Vector2{ jerkyBtn.x + 10, jerkyBtn.y + 10 }, 18, 1, WHITE);
            if (activeFoodType == 2) {
                DrawRectangleLinesEx(jerkyBtn, 3, GOLD); // ⚠️ 裝填中：套上高亮金邊！
            }

            // 4. 大補丸 (有存量顯現 GREEN，沒存量褪色為 GRAY)
            std::string txt3 = u8"大補丸 x" + std::to_string(stats->GetPillCount());
            DrawRectangleRec(pillBtn, (stats->GetPillCount() > 0) ? GREEN : GRAY); // ⚠️ 依庫存決定顏色
            DrawTextEx(chineseFont, txt3.c_str(), Vector2{ pillBtn.x + 5, pillBtn.y + 10 }, 18, 1, WHITE);
            if (activeFoodType == 3) {
                DrawRectangleLinesEx(pillBtn, 3, GOLD); // ⚠️ 裝填中：套上高亮金邊！
            }

            // 5. 打開商店按鈕
            DrawRectangleRec(buyFoodBtn, SKYBLUE);
            DrawRectangleLinesEx(buyFoodBtn, 2, BLUE);
            DrawTextEx(chineseFont, u8"打開商店", Vector2{ buyFoodBtn.x+15, buyFoodBtn.y+10 }, 18, 1, WHITE);

            // 6. 滑鼠變色特效
            if (activeFoodType == 4) DrawCircleV(mousePos, 10, ORANGE);
            else if (activeFoodType == 1) DrawCircleV(mousePos, 10, PINK);
            else if (activeFoodType == 2) DrawCircleV(mousePos, 10, BROWN);
            else if (activeFoodType == 3) DrawCircleV(mousePos, 10, LIME);

            //EndDrawing();
        }

        else if (currentState == STATE_RAISING) {
            PetStats* stats = myRaisingPet->GetStats();
            myRaisingPet->Draw(chineseFont, true);

            if (CheckCollisionPointRec(mousePos, buyFoodBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                previousState = STATE_RAISING; // 紀錄來源是養雞場
                currentState = STATE_SHOP;
            }

            if (CheckCollisionPointRec(mousePos, backToMenuBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentState = STATE_MENU;  // 切換回主選單狀態
                activeFoodType = 0;         // 清空手上可能正拿著的飼料外掛
            }

            if (CheckCollisionPointRec(mousePos, renameBth)) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentState = STATE_RAISING_NAMING;   // 切換到養雞場專屬命名畫面
                    inputText = myRaisingPet->GetName();  
                    activeFoodType = 0;                    
                }
            }

            // 飼料按鈕點擊偵測
            if (CheckCollisionPointRec(mousePos, basicFoodBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) activeFoodType = (activeFoodType == 4) ? 0 : 4;
            if (CheckCollisionPointRec(mousePos, jellyBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { if (stats->GetJellyCount() > 0) activeFoodType = (activeFoodType == 1) ? 0 : 1; }
            if (CheckCollisionPointRec(mousePos, jerkyBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { if (stats->GetJerkyCount() > 0) activeFoodType = (activeFoodType == 2) ? 0 : 2; }
            if (CheckCollisionPointRec(mousePos, pillBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { if (stats->GetPillCount() > 0) activeFoodType = (activeFoodType == 3) ? 0 : 3; }
            
            // 餵食
            if (activeFoodType > 0) {
                myRaisingPet->SetTarget(Vector2{mousePos.x - 50, mousePos.y - 50}, true);
                float dist = Vector2Distance(Vector2{myRaisingPet->GetX() + 50, myRaisingPet->GetY() + 50}, mousePos);
                    if (dist < 40.0f) { 
                    bool canEat = false; // 預設不能吃
                    if (activeFoodType == 4) {
                        if (stats->GetCoins() >= 5) {
                            stats->AddCoins(-5);     
                            canEat = true;           
                        } else {
                            myRaisingPet->Speak(u8"沒錢買基礎飼料了...", 1.5f);
                        }
                    } else {
                        canEat = stats->UseStock(activeFoodType);
                    }

                    if (canEat) myRaisingPet->Feed(activeFoodType);
                    activeFoodType = 0;
                }
            }
             
            else {
                // 沒拿食物時， follow 傳入 false，大雞雞就會啟動原本寫好的 wanderTimer 自動到處無壓力閒晃！
                myRaisingPet->SetTarget(mousePos, false); 
            }

            // 3. 更新大雞雞的時間消耗數值（飢餓度、心情）
            myRaisingPet->Update(); 

            //myPet->Draw(chineseFont, true);
            myRaisingPet->GetStats()->DrawUI(750, 20, chineseFont);
            // 基礎飼料
            DrawRectangleRec(basicFoodBtn, (activeFoodType == 4) ? GOLD : LIGHTGRAY);
            DrawTextEx(chineseFont, u8"基礎飼料", Vector2{ basicFoodBtn.x+10, basicFoodBtn.y+10 }, 18, 1, BLACK);
            if (activeFoodType == 4) DrawRectangleLinesEx(basicFoodBtn, 3, BLACK);

            // 果凍
            std::string txt1 = u8"果凍 x" + std::to_string(stats->GetJellyCount());
            DrawRectangleRec(jellyBtn, (stats->GetJellyCount() > 0) ? PINK : GRAY); 
            DrawTextEx(chineseFont, txt1.c_str(), Vector2{ jellyBtn.x + 10, jellyBtn.y + 10 }, 18, 1, WHITE);
            if (activeFoodType == 1) DrawRectangleLinesEx(jellyBtn, 3, GOLD);

            // 肉乾
            std::string txt2 = u8"肉乾 x" + std::to_string(stats->GetJerkyCount());
            DrawRectangleRec(jerkyBtn, (stats->GetJerkyCount() > 0) ? ORANGE : GRAY); 
            DrawTextEx(chineseFont, txt2.c_str(), Vector2{ jerkyBtn.x + 10, jerkyBtn.y + 10 }, 18, 1, WHITE);
            if (activeFoodType == 2) DrawRectangleLinesEx(jerkyBtn, 3, GOLD);

            // 大補丸
            std::string txt3 = u8"大補丸 x" + std::to_string(stats->GetPillCount());
            DrawRectangleRec(pillBtn, (stats->GetPillCount() > 0) ? GREEN : GRAY); 
            DrawTextEx(chineseFont, txt3.c_str(), Vector2{ pillBtn.x + 5, pillBtn.y + 10 }, 18, 1, WHITE);
            if (activeFoodType == 3) DrawRectangleLinesEx(pillBtn, 3, GOLD);

            // 補上滑鼠手持飼料時的變色圓點特效
            if (activeFoodType == 4) DrawCircleV(mousePos, 10, ORANGE);
            else if (activeFoodType == 1) DrawCircleV(mousePos, 10, PINK);
            else if (activeFoodType == 2) DrawCircleV(mousePos, 10, BROWN);
            else if (activeFoodType == 3) DrawCircleV(mousePos, 10, LIME);

            DrawRectangleRec(backToMenuBtn, LIGHTGRAY); // 按鈕底色
            DrawRectangleLinesEx(backToMenuBtn, 2, DARKGRAY); // 按鈕邊框
            DrawTextEx(chineseFont, u8"返回主選單", Vector2{ backToMenuBtn.x + 15, backToMenuBtn.y + 10 }, 18, 1, BLACK); // 按鈕文字

            DrawRectangleRec(renameBth, LIGHTGRAY);
            DrawRectangleLinesEx(renameBth, 2, DARKGRAY); 
            DrawTextEx(chineseFont, u8"改名", Vector2{ renameBth.x + 15, renameBth.y + 5 }, 24, 2, BLACK);

        }

        // 商店獨立選單狀態
        else if (currentState == STATE_SHOP) {
            //Vector2 mousePos = GetMousePosition();
            VirtualPet* currentShopPet = (previousState == STATE_RAISING) ? myRaisingPet : myPet;
            VirtualPet* otherPet = (previousState == STATE_RAISING) ? myPet : myRaisingPet;

            // 讓商店物件自己處理內部的點擊更新，如果點擊離開，Update 會回傳 true
            if (myShop->Update(mousePos, currentShopPet->GetStats())) {
                otherPet->GetStats()->Reset(); 
                otherPet->GetStats()->AddCoins(currentShopPet->GetStats()->GetCoins() - otherPet->GetStats()->GetCoins());

                currentState = previousState; // 切換回遊玩畫面，解除暫停
            }

            // 開始渲染畫面
            //BeginDrawing();
            //ClearBackground(RAYWHITE);

            // 1. 先畫出背景（因為不呼叫 Update，所以這些畫面都是定格的）
            for (const auto& enemy : enemies) {
                DrawCircleV(Vector2{enemy.x, enemy.y}, enemy.radius, PURPLE);
            }
            currentShopPet->Draw(chineseFont);
            currentShopPet->GetStats()->DrawUI(750 , 20 , chineseFont);

            // 2. 呼叫商店物件，讓它在最上層疊加半透明遮罩與大面板
            myShop->Draw(chineseFont, currentShopPet->GetStats(), width, height);

            // 3. 保持滑鼠手上的飼料特效
            if (activeFoodType == 4) DrawCircleV(mousePos, 10, ORANGE);
            else if (activeFoodType == 1) DrawCircleV(mousePos, 10, PINK);
            else if (activeFoodType == 2) DrawCircleV(mousePos, 10, BROWN);
            else if (activeFoodType == 3) DrawCircleV(mousePos, 10, LIME);
            //EndDrawing();
        }

        //遊戲結束畫面
        else if (currentState == STATE_GAMEOVER) {
            
            // 偵測是否按下 Enter 重新開始
            if (IsKeyPressed(KEY_ENTER)) {
                delete myPet; // 殺掉舊的寵物，釋放記憶體
                myPet = new VirtualPet("寵物名", "pet.png", "pet_hurt.png", 300, 200); // 誕生一隻全新的寵物，背包狀態會自動重置！

                myPet->GetStats()->Reset();
                
                enemies.clear(); // 清空舊的敵人陣列
                enemySpawnTimer = 0.0f;

                currentState = STATE_MENU; // 回到取名畫面重新開始
                inputText = ""; 
                activeFoodType = 0;

                hasUploaded = false;
            }

            if (IsKeyPressed(KEY_U)) {
                currentState = STATE_UPGRADE_SHOP; // 切換至局外商店
            }
            

            //BeginDrawing();
            //ClearBackground(RAYWHITE);

            // 印出死亡訊息與最終存活時間
            DrawTextEx(chineseFont, u8"遊戲結束 (GAME OVER)", Vector2{ 200, 200 }, 40, 2, RED);
            
            std::string highScoreText = u8"歷史最高紀錄: " + std::to_string(myPet->GetStats()->GetHighScore()) + u8" 秒";
            DrawTextEx(chineseFont, highScoreText.c_str(), Vector2{ 200, 320 }, 32, 2, GOLD); // ⚠️ 用金色渲染更好看！

            DrawTextEx(chineseFont, u8"按 Enter 鍵主選單", Vector2{ 200, 360 }, 32, 2, GRAY);
            DrawTextEx(chineseFont, u8"或 按 U 鍵 進入基因研究院永久升級", Vector2{ 200, 410 }, 24, 2, MAROON);

            //EndDrawing();
        }

        else if (currentState == STATE_UPGRADE_SHOP) {
            Vector2 mousePos = GetMousePosition();

            // 讓升級商店自己更新，如果點擊了「返回並重開局」，會執行重製並返回命名畫面
            if (myUpgradeShop->Update(mousePos, myPet->GetStats())) {
                delete myPet; 
                myPet = new VirtualPet("寵物名", "pet.png", "pet_hurt.png", 300, 200); 

                myPet->GetStats()->Reset(); // ⚠️ 這邊 Reset 就會自動讀取升級後的新 maxHealth 

                enemies.clear(); 
                enemySpawnTimer = 0.0f;
                currentState = STATE_NAMING; 
                inputText = ""; 
                activeFoodType = 0;
                hasUploaded = false;
            }

            //BeginDrawing();
            //ClearBackground(RAYWHITE);

            // 呼畫升級面板
            myUpgradeShop->Draw(chineseFont, myPet->GetStats(), width, height);

            //EndDrawing();
        }
        EndTextureMode();
        BeginDrawing();
        ClearBackground(BLACK); // 電影院黑邊背景

        DrawTexturePro(
            targetCanvas.texture,
            Rectangle{ 0, 0, (float)targetCanvas.texture.width, (float)-targetCanvas.texture.height }, 
            Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }, // 放大到當前螢幕
            Vector2{ 0, 0 }, 0.0f, WHITE
        );

        EndDrawing();
        
        }
        SaveGame();
    }






    void GameWindow::SaveGame() {
    nlohmann::json saveJson;

    // 1. 紀錄彈幕生存雞的獨立數值
    saveJson["survival_name"]   = myPet->GetName();
    saveJson["survival_time"]   = myPet->GetStats()->GetSurvivalTime();
    saveJson["survival_hunger"] = myPet->GetStats()->GetHunger();
    saveJson["survival_mood"]   = myPet->GetStats()->GetMood();
    saveJson["survival_health"] = myPet->GetStats()->GetHealth();
    saveJson["survival_jelly"]  = myPet->GetStats()->GetJellyCount();
    saveJson["survival_jerky"]  = myPet->GetStats()->GetJerkyCount();
    saveJson["survival_pill"]   = myPet->GetStats()->GetPillCount();

    // 2. 紀錄養雞場養成雞的獨立數值
    saveJson["raising_name"]   = myRaisingPet->GetName();
    saveJson["raising_time"]   = myRaisingPet->GetStats()->GetSurvivalTime();
    saveJson["raising_hunger"] = myRaisingPet->GetStats()->GetHunger();
    saveJson["raising_mood"]   = myRaisingPet->GetStats()->GetMood();
    saveJson["raising_health"] = myRaisingPet->GetStats()->GetHealth();
    saveJson["raising_jelly"]  = myRaisingPet->GetStats()->GetJellyCount();
    saveJson["raising_jerky"]  = myRaisingPet->GetStats()->GetJerkyCount();
    saveJson["raising_pill"]   = myRaisingPet->GetStats()->GetPillCount();

    // 3. 全球共用大錢包
    saveJson["shared_coins"]    = myPet->GetStats()->GetCoins();

    std::ofstream outFile("data.json");
    if (outFile.is_open()) {
        outFile << saveJson.dump(4); // 漂亮排版輸出
        outFile.close();
    }
}

// ====================================================
// 📖 ✅ 實作：從 data.json 一鍵解包還原獨立數值
// ====================================================
void GameWindow::LoadGame() {
    std::ifstream inFile("data.json");
    if (!inFile.is_open()) return; // 第一次玩沒有存檔就直接跳過

    try {
        nlohmann::json loadJson;
        inFile >> loadJson;

        // 還原生存雞的靈魂
        if (loadJson.contains("survival_name"))   myPet->SetName(loadJson["survival_name"]);
        if (loadJson.contains("survival_time"))   myPet->GetStats()->SetSurvivalTime(loadJson["survival_time"]);
        if (loadJson.contains("survival_hunger")) myPet->GetStats()->SetHunger(loadJson["survival_hunger"]);
        if (loadJson.contains("survival_mood"))   myPet->GetStats()->SetMood(loadJson["survival_mood"]);
        if (loadJson.contains("survival_health")) myPet->GetStats()->SetHealth(loadJson["survival_health"]);
        if (loadJson.contains("survival_jelly"))  myPet->GetStats()->SetJellyCount(loadJson["survival_jelly"]);
        if (loadJson.contains("survival_jerky"))  myPet->GetStats()->SetJerkyCount(loadJson["survival_jerky"]);
        if (loadJson.contains("survival_pill"))   myPet->GetStats()->SetPillCount(loadJson["survival_pill"]);

        // 還原養雞場小雞的靈魂
        if (loadJson.contains("raising_name"))   myRaisingPet->SetName(loadJson["raising_name"]);
        if (loadJson.contains("raising_time"))   myRaisingPet->GetStats()->SetSurvivalTime(loadJson["raising_time"]);
        if (loadJson.contains("raising_hunger")) myRaisingPet->GetStats()->SetHunger(loadJson["raising_hunger"]);
        if (loadJson.contains("raising_mood"))   myRaisingPet->GetStats()->SetMood(loadJson["raising_mood"]);
        if (loadJson.contains("raising_health")) myRaisingPet->GetStats()->SetHealth(loadJson["raising_health"]);
        if (loadJson.contains("raising_jelly"))  myRaisingPet->GetStats()->SetJellyCount(loadJson["raising_jelly"]);
        if (loadJson.contains("raising_jerky"))  myRaisingPet->GetStats()->SetJerkyCount(loadJson["raising_jerky"]);
        if (loadJson.contains("raising_pill"))   myRaisingPet->GetStats()->SetPillCount(loadJson["raising_pill"]);

        // 同步共用大錢包的金幣
        if (loadJson.contains("shared_coins")) {
            int coins = loadJson["shared_coins"];
            myPet->GetStats()->SetCoins(coins);
            myRaisingPet->GetStats()->SetCoins(coins);
        }
    }
    catch (...) {
        // 防止手殘改壞 JSON 導致當機的安全防護牆
    }
    inFile.close();
}