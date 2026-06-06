// 商店

#pragma once
#include "raylib.h"
#include "PetStats.h"  // 商店需要存取金幣與庫存

class Shop {
private:
    Rectangle buyFood1Btn; 
    Rectangle buyFood2Btn; 
    Rectangle buyFood3Btn; 
    Rectangle closeShopBtn; // 離開商店按鈕

public:
    Shop();
    ~Shop();

    int Update(Vector2 mousePos, PetStats* stats);
    void Draw(Font font, PetStats* stats, int screenWidth, int screenHeight);
};