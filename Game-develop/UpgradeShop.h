//  數值升級

#pragma once
#include "raylib.h"
#include "PetStats.h"

class UpgradeShop {
private:
    Rectangle upgradeHpBtn;  // 升級生命按鈕
    Rectangle closeShopBtn;  // 返回重開局按鈕
    Rectangle upgradeHungerBtn; // 
    Rectangle upgradeMoodBtn;

public:
    UpgradeShop();
    ~UpgradeShop(); 

    // 更新升級商店邏輯：點擊「返回重開局」時會回傳 true
    bool Update(Vector2 mousePos, PetStats* stats);

    // 繪製升級選單畫面
    void Draw(Font font, PetStats* stats, int screenWidth, int screenHeight);
};