//商店實作

#include "Shop.h"
#include <string>

Shop::Shop() {
    // 初始化商店內部的按鈕位置與大小
    buyFood1Btn = { 220, 180, 360, 35 };
    buyFood2Btn = { 220, 230, 360, 35 };
    buyFood3Btn = { 220, 280, 360, 35 };
    closeShopBtn = { 325, 380, 150, 40 };
}

Shop::~Shop() {
}

int Shop::Update(Vector2 mousePos, PetStats* stats) {
    int price = 40; // 黑市均一價 40 元

    // 檢查錢夠不夠
    if (stats->GetCoins() >= price) {
        if (CheckCollisionPointRec(mousePos, buyFood1Btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            stats->BuyFood(-price); // 扣除 40 元 (借用你的BuyFood傳入負值)
            stats->AddStock(1, 1);  // 極樂果凍庫存 +1
        }
        if (CheckCollisionPointRec(mousePos, buyFood2Btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            stats->BuyFood(-price);
            stats->AddStock(2, 1);  // 魔能肉乾庫存 +1
        }
        if (CheckCollisionPointRec(mousePos, buyFood3Btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            stats->BuyFood(-price);
            stats->AddStock(3, 1);  // 精煉大補丸庫存 +1
        }
    }

    // 只有點擊離開按鈕才回傳 -1 退出
    if (CheckCollisionPointRec(mousePos, closeShopBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return -1; 
    }
    return 0; 
}

void Shop::Draw(Font font, PetStats* stats, int screenWidth, int screenHeight) {
    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f)); // 半透明遮罩

    DrawRectangle(200, 100, 400, 340, LIGHTGRAY);
    DrawRectangleLines(200, 100, 400, 340, DARKGRAY);
    DrawTextEx(font, u8"大雞雞的商店", Vector2{ 310, 120 }, 32, 2, BLACK);

    // 顯示現有資產
    std::string info = u8"研究金幣: $" + std::to_string(stats->GetCoins());
    DrawTextEx(font, info.c_str(), Vector2{ 240, 155 }, 20, 2, MAROON);

    // 畫三個商品按鈕
    DrawRectangleRec(buyFood1Btn, PINK);
    DrawTextEx(font, u8"極樂果凍 $40 (心情全滿/飽食-25%/血量-20%)", Vector2{ buyFood1Btn.x+5, buyFood1Btn.y+8 }, 14, 1, BLACK);

    DrawRectangleRec(buyFood2Btn, ORANGE);
    DrawTextEx(font, u8"魔能肉乾 $40 (飽食全滿/心情-25%/血量-20%)", Vector2{ buyFood2Btn.x+5, buyFood2Btn.y+8 }, 14, 1, BLACK);

    DrawRectangleRec(buyFood3Btn, GREEN);
    DrawTextEx(font, u8"精煉大補丸 $40 (血量+10/飽食-50%/心情-50%)", Vector2{ buyFood3Btn.x+5, buyFood3Btn.y+8 }, 14, 1, BLACK);

    // 離開按鈕
    DrawRectangleRec(closeShopBtn, RED);
    DrawTextEx(font, u8"離開商店", Vector2{ closeShopBtn.x+35, closeShopBtn.y+8 }, 20, 2, WHITE);
}