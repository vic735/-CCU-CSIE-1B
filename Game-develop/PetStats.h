//數值管理設定


#pragma once
#include "raylib.h"

class PetStats{

    private:
        float hunger; //飢餓度0~100
        float mood;  //心情 0~100
        float timet;  //數值計時器


    public:
        PetStats();
        ~PetStats();

        void Update(); //時間消耗數值邏輯
        void DrawUI(int x, int y, Font font); //把數值寫條化


        //玩家互動介面
        void AddHunger(float amount);
        void AddMood(float amount);


        //取得當前數值
        float GetHunger();
        float GetMood();

};