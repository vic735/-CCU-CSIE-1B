//數值管理設定


#pragma once
#include "raylib.h"

class PetStats{

    private:
        float hunger; //飢餓度0~100
        float mood;  //心情 0~100
        float timet;  //數值計時器

        int coins;        // 擁有金幣數
        int foodCount;    // 擁有飼料數量

    public:
        PetStats();
        ~PetStats();

        void Update(); //時間消耗數值邏輯
        void DrawUI(int x, int y, Font font); //把數值寫條化


        //玩家互動介面
        void AddHunger(float amount);
        void AddMood(float amount);

        // 擴充：金幣與商店機制
        void ClickForCoin();      // 點擊獲得金幣
        bool BuyFood(int price);  // 購買飼料
        bool UseFood();           // 使用飼料（扣庫存)

        int GetFoodCount();       // 取得目前飼料數
        int GetCoins();           //讀檔金幣
        // 擴充：JSON 存取檔
        void SaveSettings();      // 儲存至存檔
        void LoadSettings();      // 讀取存檔


        //取得當前數值
        float GetHunger();
        float GetMood();

};