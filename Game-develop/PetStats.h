//數值管理設定.


#pragma once
#include "raylib.h"

class PetStats{

    private:
        float hunger; //飢餓度0~100
        float mood;  //心情 0~100
        float timet;  //數值計時器

        int survivalTime; //計時器
        float health;//血條0~100

        int coins;        // 擁有金幣數
        int foodCount;    // 擁有飼料數量
        float maxHealth;  // 最大血量上限 (預設 100.0f)
        float maxHunger;
        float maxMood;

        int highScore;    //歷史分數

        int jellyCount;  
        int jerkyCount;  
        int pillCount;


    public:
        PetStats();
        ~PetStats();

        void Update(bool isChasing); //時間消耗數值邏輯
        void DrawUI(int x, int y, Font font); //把數值寫條化


        //玩家互動介面
        void AddHunger(float amount);
        void AddMood(float amount);
        void TakeDamage(float amount);  //碰觸傷害


        //取得當前數值
        float GetHunger();
        float GetMood();
        int GetSurvivalTime(); //計時器
        float GetHealth();//血條

        // 擴充：金幣與商店機制
        bool BuyFood(int price);  // 購買飼料

        int GetFoodCount();       // 取得目前飼料數
        int GetCoins();           //讀檔金幣
        // 擴充：JSON 存取檔
        void SaveSettings();      // 儲存至存檔
        void LoadSettings();      // 讀取存檔
        void AddCoins(int amount);

        void Reset();

        int GetHighScore();       // 取得歷史最高分數

        float GetMaxHealth();       //取得血量上限
        float GetMaxHunger();
        float GetMaxMood();
        bool BuyUpgradeMaxHealth(int price, float amount);      //永久提升血量上限
        bool BuyUpgradeMaxHunger(int price, float amount); 
        bool BuyUpgradeMaxMood(int price, float amount);

        void EatSpecialFood(int type); //統一處理四種食物效果

        // 庫存控制介面
        int GetJellyCount();
        int GetJerkyCount();
        int GetPillCount();
        void AddStock(int type, int amount);  // 商店購買時加庫存
        bool UseStock(int type);

};